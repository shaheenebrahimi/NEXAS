// Robot.h
#ifndef ROBOT_H
#define ROBOT_H

#include <Servo.h>
#include <math.h>
#include "protocol.h"

struct ServoConfig {
    int pin;
    const char* name;
    int length;

    ServoConfig(int length = 0, int pin = -1, const char* name = "") 
        : length(length), pin(pin), name(name) {}
};

class Robot {
private:
    const ServoConfig (&config)[NUM_JOINTS];
    Servo servos[NUM_JOINTS];
    double pose[NUM_JOINTS];
    bool elbowUp;

public:
    Robot(const ServoConfig (&config)[NUM_JOINTS], bool elbowUp = true) : config(config), elbowUp(elbowUp) {}

    void begin() {
        for (int i = 0; i < NUM_JOINTS; i++) {
            servos[i].attach(config[i].pin);
        }
    }

    void moveJoint(int joint, int angle) {
        if (joint < 0 || joint >= NUM_JOINTS) return;
        angle = constrain(angle, 0, 180);
        servos[joint].write(angle);
    }

    void moveToCurrentPose() {
        for (int i = 0; i < NUM_JOINTS; ++i) {
            moveJoint(i, this->pose[i]);
        }
    }

    bool setPose(double x, double y, double phi) {
        bool reachable = computeIK(x, y, phi);

        if (!reachable) return false;

        moveToCurrentPose();

        return true;
    }

    // Inverse Kinematics for a 3R planar arm
    // Inputs:
    //   x, y     : target end-effector position
    //   phi      : desired end-effector orientation (radians)
    // Outputs:
    //   t1, t2, t3 : joint angles (radians)
    // Returns:
    //   true if target reachable, false otherwise
    bool computeIK(const double x, const double y, const double phi) {
        // Variables
        const double &l1 = this->config[0].length;
        const double &l2 = this->config[1].length;
        const double &l3 = this->config[2].length;
        double &t1 = this->pose[0];
        double &t2 = this->pose[1];
        double &t3 = this->pose[2];

        // Step 1: compute wrist position (subtract l3 along phi)
        double xw = x - l3 * cos(phi);
        double yw = y - l3 * sin(phi);

        // Step 2: distance from base to wrist
        double r = hypot(xw, yw);

        // Step 3: check reachability before doing any trig
        if (r > (l1 + l2) || r < fabs(l1 - l2)) {
            return false; // target unreachable
        }

        // Step 4: law of cosines for elbow angle
        double cos_t2 = (r*r - l1*l1 - l2*l2) / (2.0 * l1 * l2);

        // Clamp to [-1, 1] to avoid floating-point errors
        if (cos_t2 > 1.0) cos_t2 = 1.0;
        else if (cos_t2 < -1.0) cos_t2 = -1.0;

        // Step 5: determine elbow-up or elbow-down
        double sin_t2 = sqrt(fmax(0.0, 1.0 - cos_t2 * cos_t2));
        if (!elbowUp) sin_t2 = -sin_t2;

        // Step 6: compute angles
        t2 = atan2(sin_t2, cos_t2);

        // Planar shoulder angle
        double A = l1 + l2 * cos_t2;
        double B = l2 * sin_t2;
        t1 = atan2(yw, xw) - atan2(B, A);

        // Wrist angle to satisfy end-effector orientation
        t3 = phi - t1 - t2;

        return true; // valid solution
    }


};

#endif // ROBOT_H
