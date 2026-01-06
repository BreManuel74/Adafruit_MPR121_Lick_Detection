#include <Wire.h>
#include "Adafruit_MPR121.h"

// Create sensor instances
Adafruit_MPR121 cap1 = Adafruit_MPR121();
Adafruit_MPR121 cap2 = Adafruit_MPR121();

uint16_t lasttouched1 = 0;
uint16_t currtouched1 = 0;
uint16_t lasttouched2 = 0;
uint16_t currtouched2 = 0;

void setup() {
  Serial.begin(115200);
  
  // Initialize MPR121 sensors
  if (!cap2.begin(0x5A)) {
    Serial.println("cap2 (0x5A) not found!");
    while (1); // Stop if sensor not found
  }

  if (!cap1.begin(0x5B)) {
    Serial.println("cap1 (0x5B) not found!");
    while (1); // Stop if sensor not found
  }
  // Run autoconfig once to establish baseline
  cap1.setAutoconfig(true);
  cap2.setAutoconfig(true);
  delay(100);

  // Freeze parameters
  cap1.setAutoconfig(false);
  cap2.setAutoconfig(false);

  // Manually lock CDC and CDT for long BNC cables
  cap1.writeRegister(0x5C, 63);   // CDC: max charge current
  cap2.writeRegister(0x5C, 63);

  // CONFIG2: CDT = 32us (bits 7–5 = 111)
  cap1.writeRegister(0x5D, 0b11100000);
  cap2.writeRegister(0x5D, 0b11100000);

  // Lower touch/release thresholds
  cap1.setThresholds(6, 3);
  cap2.setThresholds(6, 3);

}

void loop() {
  // Get touch states (optional — not used in this example)
  currtouched1 = cap1.touched();
  currtouched2 = cap2.touched();

  lasttouched1 = currtouched1;
  lasttouched2 = currtouched2;  

  // Print timestamp in milliseconds since Arduino started
  Serial.print("T:");
  Serial.print(millis());
  Serial.print(" B#1: ");

  // Print filtered data from cap1
  for (uint8_t i = 0; i < 12; i++) {
    Serial.print(cap1.filteredData(i));
    Serial.print(" ");
  }

  Serial.print("B#2: ");

  // Print filtered data from cap2
  for (uint8_t i = 0; i < 12; i++) {
    Serial.print(cap2.filteredData(i));
    Serial.print(" ");
  }

  Serial.println(); // End of line
  delay(20);        // 
}
