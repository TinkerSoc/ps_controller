#include <inttypes.h>

#ifndef STEERING_H
#define STEERING_H

#define STEERING_MAX 1024
#define STEERING_MIN (-1024)

#define STEERING_DIR_LEFT LOW
#define STEERING_DIR_RIGHT HIGH

#define STEERING_MICROS_PER_SECOND (1000000)

class Steering {
private:
  static Steering* instance;
  // limit switches
  bool limit_left;
  bool limit_right;

  // internal state
  bool calibrated;
  bool enabled;
  int interval;

  // stepper info
  int min_pos;
  int max_pos;
  int cur_pos;
  int dest;

  // pins
  int pin_limit_left;
  int pin_limit_right;
  int pin_dir;
  int pin_pulse;

public:  
  Steering(int pin_limit_left, int pin_limit_right, int pin_dir, int pin_pulse, uint32_t freq_hz);

  void enable();
  void disable();

  int calibrate();

  bool step(int dir);
  bool step_left();
  bool step_right();

  void loop();
  void move(int dest);
  void move_rel(int rel_dest);

  void isr_left();
  void isr_right();

  static void isr_cb_left();
  static void isr_cb_right();

};

#endif // STEERING_H
