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
  cap1.setAutoconfig(true);
  cap2.setAutoconfig(true);
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
  delay(30);        // ~20Hz logging rate
}
