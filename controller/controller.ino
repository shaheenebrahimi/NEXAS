#define ARDUINO_ID "robotarm"
#define BAUD_RATE 9600

#define START_BYTE 0xAA
#define CHECKSUM_MASK 0xFF
#define PACKET_SIZE 5

void setup() {
  // put your setup code here, to run once:
  Serial.begin(BAUD_RATE);
  
  // Wait until serial port is actually ready
  while (!Serial) { ; }

  // Notify Python that Arduino is ready
  Serial.println("READY");
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

      Serial.println("Received packet");

      if (checksum == computedChecksum) {
        Serial.println("Processing command");
      }
      else {
        Serial.println("Failed Checksum");
      }
    }

  }
}
