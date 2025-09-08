#include <Servo.h>

#include "protocol.h"

const int servoPin = 9;

// Declarations
Servo limb;

void setup() {
  // Initialize serial connection
  Serial.begin(BAUD_RATE);

  // Attach pins
  limb.attach(servoPin);
  
  // Wait until serial port is actually ready
  while (!Serial) { ; }

  // Notify Python that Arduino is ready
  Serial.println("READY");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() >= PACKET_SIZE) {
    uint8_t start = Serial.read();
    if (start == START_BYTE) {
      uint8_t cmd = Serial.read();
      uint8_t joint = Serial.read();
      uint8_t angle = Serial.read();
      uint8_t checksum = Serial.read();

      uint8_t computedChecksum = (cmd + joint + angle) & CHECKSUM_MASK;

      // Serial.println("Received packet");

      if (checksum == computedChecksum) {
        Serial.println("Processing command");
        angle = constrain(angle, 0, 180);
        limb.write(angle);
      }
      else {
        Serial.println("Failed Checksum");
      }
    }

  }
}
