// Robot.h
#ifndef ROBOT_H
#define ROBOT_H

#include <Servo.h>

#define MAX_SERVOS 2

struct ServoConfig {
    int pin;
    const char* name;

    ServoConfig(int pin = -1, const char* name = "") 
        : pin(pin), name(name) {}
};

class Robot {
private:
    int servoCount = 0;
    Servo servos[MAX_SERVOS];
    const ServoConfig* config[MAX_SERVOS];

public:
    Robot() {}

    Robot(const ServoConfig config[], int count) {
        count = min(count, MAX_SERVOS);
        this->servoCount = count;
        for (int i = 0; i < count; ++i) {
            this->config[i] = &config[i];
        }
    }

    void begin() {
        for (int i = 0; i < servoCount; i++) {
            servos[i].attach(config[i]->pin);
            // servos[i].write(90); // initial angle
        }
    }

    void moveJoint(int joint, int angle) {
        if (joint < 0 || joint >= servoCount) return;
        angle = constrain(angle, 0, 180);
        servos[joint].write(angle);
    }
};

#endif // ROBOT_H
