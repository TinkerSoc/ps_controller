#include <Arduino.h>
#include "Brake.hpp"

Brake *Brake::instance = NULL;

Brake::Brake(int pin_enable) : enabled(true), pin_enable(pin_enable) {
  digitalWrite(pin_enable, BRAKE_VAL_ENABLE);
}

void Brake::enable() {
  digitalWrite(pin_enable, BRAKE_VAL_ENABLE);
}

void Brake::disable() {
  digitalWrite(pin_enable, BRAKE_VAL_DISABLE);
}
