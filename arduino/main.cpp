#include <HardwareSerial.h>
#include <Arduino.h>
extern HardwareSerial Serial;
/*
#ifdef DEBUG
#define debug_begin(x) do { Serial.print("# "); } while (0)
#define debug_end(x) do { Serial.print("\r\n"); } while (0)
#define debug(x) do { Serial.print(x); } while (0)
#else
#define debug_begin(x) do {} while (0)
#define debug_end(x) do {} while (0)
#define debug(x) do {} while (0)
#endif
#define debugln(x) do { debug_begin(); debug(x); debug_end(); } while(0)
*/

#include "command.h"

#include <stdio.h>
#include <inttypes.h>

#include "Steering.hpp"
#include "Throttle.hpp"
#include "Brake.hpp"

// serial commands

#define BUFFER_SIZE (128)
#define BAUD_RATE (115200)

char buf[BUFFER_SIZE];
char* cur;


#define STEERING_PIN_LIMIT_LEFT (2)
#define STEERING_PIN_LIMIT_RIGHT (3)
#define STEERING_PIN_DIR (PIN_A11)
#define STEERING_PIN_PULSE (PIN_A10)
#define STEERING_FREQ_HZ (200000)

Steering steering(STEERING_PIN_LIMIT_LEFT,
		  STEERING_PIN_LIMIT_RIGHT,
		  STEERING_PIN_DIR,
		  STEERING_PIN_PULSE,
		  STEERING_FREQ_HZ);

#define THROTTLE_PIN_DIR (86) // A11
#define THROTTLE_PIN_PWM (87) // A10
#define THROTTLE_PIN_ENABLE (85) // A12

Throttle throttle(THROTTLE_PIN_DIR,
		  THROTTLE_PIN_PWM,
		  THROTTLE_PIN_ENABLE);

#define BRAKE_PIN (53) // who the fuck knows?

Brake brake(BRAKE_PIN);


void setup() {
  Serial.begin(BAUD_RATE);
  
  // Serial command parsing
  cur = buf;

  brake.enable();
  steering.disable();
  throttle.disable();
}


bool read_command(command_t *cmd) {
  while (Serial.available()) {
    if (! (cur < buf+(BUFFER_SIZE-1))) {
      cur = buf;
      return false;
    }
    *cur = Serial.read();
    
    if (*cur == '\n') {
      *(++cur) = '\0';
      cur = buf;
      return command_parse(cmd, buf) != buf;
    } else {
      cur++;
      return false;
    }
  }
  return false;
}

void loop() {
  //steering.loop();
  command_t cmd;
  if (read_command(&cmd)) {
    switch(cmd.name) {
    case CMD_BRAKE:
      // TODO
      break;
    case CMD_FREEWHEEL:
      // TODO
      break;
    case CMD_INIT:
      //steering.enable();
      //steering.calibrate();
      throttle.enable();
      break;
    case CMD_STEER_REL:
      //steering.move_rel(cmd.value);
      break;
    case CMD_STEER_ABS:
      //steering.move(cmd.value);
      break;
    case CMD_THROTTLE_ABS: {
      throttle.set(cmd.value);
      break;
    }
    default:
      break;
    }
  }
}


