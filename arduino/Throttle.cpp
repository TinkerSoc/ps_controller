#include <Arduino.h>
#include <inttypes.h>

#include "Throttle.hpp"

Throttle *Throttle::instance = NULL;

Throttle::Throttle(int pin_dir, int pin_pwm, int pin_enable) :
  enabled(false),
  pin_dir(pin_dir), pin_pwm(pin_pwm), pin_enable(pin_enable)
{
  digitalWrite(pin_enable, THROTTLE_DISABLE_VAL);
  digitalWrite(pin_pwm, LOW);
  instance = this;
}

void Throttle::enable() {
  digitalWrite(pin_pwm, LOW);
  digitalWrite(pin_enable, THROTTLE_ENABLE_VAL);
  enabled = true;
}

void Throttle::disable() {
  digitalWrite(pin_enable, THROTTLE_DISABLE_VAL);
  digitalWrite(pin_pwm, LOW);
  enabled = false;
}

bool Throttle::set(int val) {
  if (!enabled) {
    return false;
  }
  
  int v = constrain(map(val, THROTTLE_MIN, THROTTLE_MAX, -255, 255), -255, 255);
  if (v < 0) {
    digitalWrite(pin_dir, THROTTLE_DIR_LEFT);
    analogWrite(pin_pwm, abs(v));
  } else if (v > 0) {
    digitalWrite(pin_dir, THROTTLE_DIR_RIGHT);
    analogWrite(pin_pwm, v);
  } else {
    digitalWrite(pin_pwm, LOW);
  }
  return true;
}
