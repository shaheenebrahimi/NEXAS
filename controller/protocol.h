// Auto-generated file. Do not edit manually.

#define BAUD_RATE 9600
#define START_BYTE 170
#define CHECKSUM_MASK 255
#define PACKET_SIZE 5

enum class COMMANDS {
   MOVE = 1,
   STOP = 2,
   CALIBRATE = 3
};
enum class JOINTS {
   BASE = 1,
   LOWER = 2,
   UPPER = 3,
   GRIPPER = 4
};
