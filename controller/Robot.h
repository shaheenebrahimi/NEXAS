// Robot.h
#ifndef ROBOT_H
#define ROBOT_H

#include <Servo.h>
#include "ServoConfig.h"

class Robot {
private:
    Servo servo;
    ServoConfig config;

public:
    Robot() {}

    Robot(const ServoConfig &config)
        : config(config) {}

    void begin() {
        this->servo.attach(config.pin);
    }

    void moveJoint(int angle) {
        angle = constrain(angle, 0, 180);
        servo.write(angle);
    }
};

#endif // ROBOT_H
