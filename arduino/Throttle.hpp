#include <inttypes.h>

#ifndef THROTTLE_H
#define THROTTLE_H

#define THROTTLE_MAX 1024
#define THROTTLE_MIN (-1024)

#define THROTTLE_DIR_LEFT LOW
#define THROTTLE_DIR_RIGHT HIGH

#define THROTTLE_ENABLE_VAL LOW
#define THROTTLE_DISABLE_VAL HIGH

class Throttle {
private:
  static Throttle* instance;

  bool enabled;

  // pins
  int pin_dir;
  int pin_pwm;
  int pin_enable;
 
public:
  Throttle(int pin_dir, int pin_pwm, int pin_enable);

  void enable();
  void disable();

  bool set(int val);
};

#endif // THROTTLE_H
