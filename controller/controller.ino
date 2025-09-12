#include <Servo.h>
#include "protocol.h"
#include "Robot.h"

// Structs
struct Packet {
    uint8_t command;
    uint8_t joint;
    uint8_t angle;
};

// Config
ServoConfig config[] = {
  ServoConfig(9, "base")
};
int numServos = sizeof(config) / sizeof(config[0]);
Robot robot(config, numServos);

Packet readPacket() {
    if (Serial.available() < PACKET_SIZE) return {0,0,0};  // wait until full packet

    uint8_t start = Serial.read();
    if (start != START_BYTE) return {0,0,0};  // invalid start byte

    uint8_t cmd = Serial.read();
    uint8_t joint = Serial.read();
    uint8_t angle = Serial.read();
    uint8_t checksum = Serial.read();
    uint8_t computedChecksum = (cmd + joint + angle) & CHECKSUM_MASK;

    if (checksum != computedChecksum) {
        Serial.println("Failed Checksum!");
        return {0,0,0};
    }

    // Debug
    Serial.print("Received packet: ");
    Serial.print("CMD=");
    Serial.print(cmd);
    Serial.print(", JOINT=");
    Serial.print(joint);
    Serial.print(", ANGLE=");
    Serial.print(angle);
    Serial.print(", CHECKSUM=");
    Serial.print((checksum == computedChecksum) ? "PASS" : "FAIL");

    return {cmd, joint, angle};
}

void setup() {
  // Initiate serial and servos
  Serial.begin(BAUD_RATE);
  robot.begin();
  
  // Wait until serial port is actually ready
  while (!Serial) { ; }

  // Notify Python that Arduino is ready
  Serial.println("READY");
}

void loop() {
    if (Serial.available() < PACKET_SIZE) return;

    Packet packet = readPacket();
    if (packet.command == 0) return; // invalid packet

    COMMANDS processedCmd = static_cast<COMMANDS>(packet.command);
    switch (processedCmd) {
        case COMMANDS::MOVE:
            Serial.print(" - Moving servo to angle: ");
            Serial.println(packet.angle);
            robot.moveJoint(packet.joint, packet.angle);
            break;
        default:
            break;
    }
}

