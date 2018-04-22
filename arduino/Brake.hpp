#include <inttypes.h>

#ifndef BRAKE_H
#define BRAKE_H

#define BRAKE_VAL_ENABLE LOW
#define BRAKE_VAL_DISABLE HIGH

class Brake {
private:
  static Brake* instance;

  bool enabled;
  int pin_enable;

 public:
  Brake(int pin_enable);

  void enable();
  void disable();
};

#endif // BRAKE_H
