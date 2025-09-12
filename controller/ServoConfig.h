// ServoConfig.h
#ifndef SERVOCONFIG_H
#define SERVOCONFIG_H

#include <Servo.h>

struct ServoConfig {
  int pin;
  const char* name;

  ServoConfig(int pin = -1, const char* name = "") 
        : pin(pin), name(name) {}
};

#endif // SERVOCONFIG_H