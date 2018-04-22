#include <Arduino.h>
#include <inttypes.h>

#include "Steering.hpp"

Steering *Steering::instance = NULL;

Steering::Steering(int pin_limit_left, int pin_limit_right,
		   int pin_dir, int pin_pulse, uint32_t freq_hz) :
  limit_left(false), limit_right(false), enabled(false),
  pin_limit_left(pin_limit_left), pin_limit_right(pin_limit_right),
  pin_dir(pin_dir), pin_pulse(pin_pulse)
{
  instance = this;

  interval = STEERING_MICROS_PER_SECOND / freq_hz;

  attachInterrupt(digitalPinToInterrupt(pin_limit_left), Steering::isr_cb_left, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pin_limit_right), Steering::isr_cb_right, CHANGE);
}

void Steering::enable() {
  enabled = true;
}

void Steering::disable() {
  enabled = false;
}

int Steering::calibrate() {
  int steps = 0;
  while(step_left());
  
  while(step_right()) {
    steps++;
  }

  cur_pos = steps/2;
  max_pos = steps/2;
  min_pos = 0 - max_pos;
  dest = 0;
  calibrated = true;
  return steps;
}

bool Steering::step(int dir) {
  if (!enabled) {
    return false;
  }

  digitalWrite(pin_dir, dir);
  digitalWrite(pin_pulse, HIGH);
  delayMicroseconds(interval);
  digitalWrite(pin_pulse, LOW);
  delayMicroseconds(interval);
  
  return true;
}

bool Steering::step_left() {
  if (!limit_left && step(LOW)) {
    cur_pos--;
    return true;
  }
  return false;
}
 
bool Steering::step_right() {
  if (!limit_right && step(HIGH)) {
    cur_pos++;
    return true;
  }
  return false;
}

void Steering::loop() {
  if (dest < cur_pos) {
    step_left();
  } else if (dest > cur_pos) {
    step_right();
  }
}

void Steering::move(int new_dest) {
  int v = map(new_dest, STEERING_MIN, STEERING_MAX, min_pos, max_pos);
  dest = constrain(v, min_pos, max_pos);
}

void Steering::move_rel(int rel_dest) {
  int v = map(rel_dest, STEERING_MIN*2, STEERING_MAX*2, min_pos*2, max_pos*2);
  dest = constrain(dest + v, min_pos, max_pos);
} 
 
void Steering::isr_left() {
  limit_left = (digitalRead(pin_limit_left) == LOW);
}

void Steering::isr_right() {
  limit_right = (digitalRead(pin_limit_right) == LOW);
}

void Steering::isr_cb_left() {
  Steering::instance->isr_left();
}

void Steering::isr_cb_right() {
  instance->isr_right();
}
