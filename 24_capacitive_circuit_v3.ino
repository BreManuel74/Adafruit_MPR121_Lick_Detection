#include <Wire.h>
#include "Adafruit_MPR121.h"

// Create sensor instances
Adafruit_MPR121 cap1 = Adafruit_MPR121();
Adafruit_MPR121 cap2 = Adafruit_MPR121();

// Fast baseline variables (one per electrode)
float lickBaseline1[12] = {0};
float lickBaseline2[12] = {0};

// Baseline recovery speed
// 0.1 = slower recovery
// 0.2–0.3 = good for mouse licks
const float alpha = 0.2;

void setup() {
  Serial.begin(115200);

  if (!cap2.begin(0x5A)) {
    Serial.println("cap2 (0x5A) not found!");
    while (1);
  }

  if (!cap1.begin(0x5B)) {
    Serial.println("cap1 (0x5B) not found!");
    while (1);
  }

  // Run autoconfig once to establish baseline
  cap1.setAutoconfig(true);
  cap2.setAutoconfig(true);
  delay(100);

  // Freeze parameters
  cap1.setAutoconfig(false);
  cap2.setAutoconfig(false);

  // Lock CDC (charge current)
  cap1.writeRegister(0x5C, 63);
  cap2.writeRegister(0x5C, 63);

  // Lock CDT (charge time = 32 µs)
  cap1.writeRegister(0x5D, 0b11100000);
  cap2.writeRegister(0x5D, 0b11100000);

  // Lower thresholds (not used directly for lick detection)
  cap1.setThresholds(6, 3);
  cap2.setThresholds(6, 3);
}

void loop() {
  // Timestamp (use micros for higher temporal precision)
  Serial.print("T:");
  Serial.print(micros());
  Serial.print(" ");

  // ===== Board 1 =====
  Serial.print("B#1: ");
  for (uint8_t i = 0; i < 12; i++) {
    int raw = cap1.baselineData(i) - cap1.filteredData(i);

    // Fast baseline restoration (high-pass behavior)
    lickBaseline1[i] = (1.0 - alpha) * lickBaseline1[i] + alpha * raw;
    float lickSignal = raw - lickBaseline1[i];

    Serial.print(lickSignal);
    Serial.print(" ");
  }

  // ===== Board 2 =====
  Serial.print("B#2: ");
  for (uint8_t i = 0; i < 12; i++) {
    int raw = cap2.baselineData(i) - cap2.filteredData(i);

    lickBaseline2[i] = (1.0 - alpha) * lickBaseline2[i] + alpha * raw;
    float lickSignal = raw - lickBaseline2[i];

    Serial.print(raw);
    Serial.print(" ");
  }

  Serial.println();

  // Run as fast as possible (important for lick detection)
  //delay(4);   // optional, ~1 kHz
}
