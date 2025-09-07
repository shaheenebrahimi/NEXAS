#define START_BYTE 0xAA
#define CHECKSUM_MASK 0xFF
#define SERIAL_PORT 9600
#define PACKET_SIZE 5 // START_BYTE 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(SERIAL_PORT);
  Serial.println("Arduino Serial Stream Established");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() >= PACKET_SIZE) {
    byte start = Serial.read();
    if (start == START_BYTE) {
      byte cmd = Serial.read();
      byte joint = Serial.read();
      byte angle = Serial.read();
      byte checksum = Serial.read();

      byte computedChecksum = (cmd + joint + angle) & CHECKSUM_MASK;

      if (checksum == computedChecksum) {
        Serial.println("Processing command");
      }
      else {
        Serial.println("Failed Checksum");
      }
    }

  }
}
