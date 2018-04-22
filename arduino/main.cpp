#include <HardwareSerial.h>
#include <Arduino.h>
extern HardwareSerial Serial;

// Command - Version: Latest 
#include "command.h"

#include <assert.h>
#include <stdio.h>
#include <inttypes.h>

// serial commands

#define BUFFER_SIZE (128)
#define BAUD_RATE (115200)

// steering

#define STEERING_ABS_MAX 1024
#define STEERING_ABS_MIN (-1024)


#define PIN_STEERING_PEND   (89) // A8
#define PIN_STEERING_ALARM  (88) // A9
#define PIN_STEERING_PULSE  (87) // A10
#define PIN_STEERING_DIR    (86) // A11
#define PIN_STEERING_ENABLE (85) // A12

#define STEERING_GAP 5
#define STEERING_MIN_TICK   (1000000 / 200000)
#define STEERING_DIR_LEFT HIGH
#define STEERING_DIR_RIGHT LOW

#define PIN_TRANSMISSION_BRAKE (53)

#define PIN_STEERING_LIMIT_L 2
#define PIN_STEERING_LIMIT_R 3

// throttle

//#define PIN_THROTTLE 9
#define PIN_THROTTLE (LED_BUILTIN)
#define PIN_THROTTLE_DIR 10

#define THROTTLE_ABS_MAX 1024
#define THROTTLE_ABS_MIN (-1024)


//==========================================================
// Serial Commands
//==========================================================

char buf[BUFFER_SIZE];
char* cur;

//==========================================================
// Steering
//==========================================================

volatile bool steer_left_limit;
volatile bool steer_right_limit;

int steering_min;
int steering_max;
int steering_cur;
int steering_dest;

void steer_limit_l_isr() {
  steer_left_limit = (digitalRead(PIN_STEERING_LIMIT_L) == LOW);
}

void steer_limit_r_isr() {
  steer_right_limit = (digitalRead(PIN_STEERING_LIMIT_R) == LOW);
}

void steer_pulse() {
  digitalWrite(PIN_STEERING_PULSE, HIGH);
  delayMicroseconds(STEERING_MIN_TICK);
  digitalWrite(PIN_STEERING_PULSE, LOW);
  delayMicroseconds(STEERING_MIN_TICK);
}

bool steer_left() {
  if (steer_left_limit) {
    return false;
  }
  
  digitalWrite(PIN_STEERING_DIR, STEERING_DIR_LEFT);
  steer_pulse();
  steering_cur--;
  return true;
}

bool steer_right() {
  if (steer_right_limit) {
    return false;
  }
  
  digitalWrite(PIN_STEERING_DIR, STEERING_DIR_RIGHT);
  steer_pulse();
  steering_cur++;
  return true;
}

void steer_loop() {
  if (steering_dest < steering_cur) {
    steer_left();
  } else if (steering_dest > steering_cur) {
    steer_right();
  }
}

void steering_init() {
  int steps;
  while (steer_left()) {
    delay(5);
  }
  while (steer_right()) {
    delay(5);
    steps++;
  }
  
  steering_cur = steps / 2;
  steering_max = (steps / 2) - STEERING_GAP;
  steering_min = 0 - steering_max;
  
  steering_dest = 0;
}



//==========================================================
// Throttle
//==========================================================

bool throttle_enabled;

void throttle_enable() {
  digitalWrite(PIN_TRANSMISSION_BRAKE, HIGH);
  throttle_enabled = true;
}

bool throttle_set(int n) {
  if (!throttle_enabled) {
    return false;
  }

  digitalWrite(PIN_THROTTLE_DIR, n < 0 ? STEERING_DIR_LEFT : STEERING_DIR_RIGHT);
  analogWrite(PIN_THROTTLE, abs(n));
  return true;
}

int map_throttle_value(int v) {
  int x = map(v, THROTTLE_ABS_MIN, THROTTLE_ABS_MAX, -255, 255);
  return constrain(x, -255, 255);
}



//==========================================================
// Main
//==========================================================

void setup() {
  Serial.begin(BAUD_RATE);
  
  // Steering
  
  steering_min = 0;
  steering_max = 0;
  steering_dest = 0;
  steering_cur = 0;
  pinMode(PIN_STEERING_PULSE, OUTPUT);
  pinMode(PIN_STEERING_DIR, OUTPUT);
  digitalWrite(PIN_STEERING_PULSE, LOW);
  digitalWrite(PIN_STEERING_DIR, LOW);
  pinMode(PIN_STEERING_LIMIT_L, INPUT);
  pinMode(PIN_STEERING_LIMIT_R, INPUT);
  
  attachInterrupt(
    digitalPinToInterrupt(PIN_STEERING_LIMIT_L),
    steer_limit_l_isr,
    CHANGE);
    
  attachInterrupt(
    digitalPinToInterrupt(PIN_STEERING_LIMIT_R),
    steer_limit_r_isr,
    CHANGE);
    
  // Throttle
  pinMode(PIN_THROTTLE, OUTPUT);
  pinMode(PIN_THROTTLE_DIR, OUTPUT);
  throttle_enabled = false;
  
  
  // Serial command parsing
  cur = buf;
}


bool read_command(command_t *cmd) {
  while (Serial.available()) {
    if (! (cur < buf+(BUFFER_SIZE-1))) {
      // discard overflows
      // TODO: maybe use ring buffer
      cur = buf;
      return false;
    }
    
    // read char into buffer
    *cur = Serial.read();
    
    if (*cur == '\n') {
      // null terminate after a newline
      *(++cur) = '\0';
      // reset cursor
      cur = buf;
      // call command parser
      return command_parse(cmd, buf) != buf;
    } else {
      // continue filling buffer
      cur++;
      return false;
    }
  }
  return false;
}

int map_steering_value(int v) {
  int x =  map(v, STEERING_ABS_MIN, STEERING_ABS_MAX, steering_min, steering_max);
  return constrain(x, steering_min, steering_max);
}



void loop() {
  steer_loop();
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
      //if(cmd.value == 1) {
      //steering_init();
      //}
      throttle_enable();
      break;
    case CMD_STEER_REL:
      steering_dest += map_steering_value(cmd.value);
      break;
    case CMD_STEER_ABS:
      steering_dest = map_steering_value(cmd.value);
      break;
    case CMD_THROTTLE_ABS: {
      int v = map_throttle_value(cmd.value);
      throttle_set(v);
      // Serial.print("Throttle: ");
      //Serial.print(v);
      //Serial.print("\r\n");
      //delay(5);
      break;
    }
    default:
      break;
    }
  }
}

