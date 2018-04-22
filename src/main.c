#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __STDC_NO_THREADS__
#include "c11threads.h"
#else
#include <threads.h>
#endif

#include "controller.h"
#include "serial.h"

#define UPDATES_PER_SEC         (60)
#define UPDATE_PERIOD           (1000000000 / UPDATES_PER_SEC)
#define REVERSE_BUTTON          (0x130)
#define STEERING_AXIS           (0)
#define THROTTLE_AXIS           (5)

#define BTN_REINIT              (0x134)
#define BTN_SPEED_LMT_UP        (0x137)
#define BTN_SPEED_LMT_DOWN      (0x136)
#define MAX_SPEED               (4)

int btnUp = 0;
int btnDown = 0;
int btnReInit = 0;
int limit = 0;


typedef struct {
  char * filename;
  int baud;
} serial_config_t;
serial_config_t serial_config;

thrd_start_t controller(void * filename) {
  FILE * fp = initialise((char*) filename);
  printf("Device name: %s\n", device_name);

  while(1) update_states(fp);  
}

void set_xbox_leds(int l) {
  int cmds[] = { 6, 7, 8, 9, 0 };
  FILE * fp = fopen("/sys/class/leds/xpad0/brightness", "w");
  if(fp != NULL) {
    fprintf(fp, "%d\n", cmds[l]);
    fclose(fp);
  }
}

thrd_start_t send_data(void * config) {
  FILE * ser = serial_connect(((serial_config_t*)config)->filename,
			      ((serial_config_t*)config)->baud);

  set_xbox_leds(5);

  send_initialise(ser);
  xtime delay = { 2, 0 };
  thrd_sleep(&delay);

  send_initialise(ser);
  thrd_sleep(&delay);

  set_xbox_leds(limit);
  
  while(1) {
    if(get_button(BTN_SPEED_LMT_UP) == 1) {
      if(btnUp == 0) {
	limit += MAX_SPEED / 4;
	limit %= MAX_SPEED;
	set_xbox_leds(limit);
      }
      btnUp = 1;
    } else {
      btnUp = 0;
    }
    
    if(get_button(BTN_SPEED_LMT_DOWN) == 1) {
      if(btnDown == 0) {
	limit += (MAX_SPEED / 4) * 3;
	limit %= MAX_SPEED;
	set_xbox_leds(limit);
      }
      btnDown = 1;
    } else {
      btnDown = 0;
    }

    if(get_button(BTN_REINIT) == 1) {
      if(btnReInit == 0) {
	set_xbox_leds(5);
	send_initialise(ser);
	xtime delay = { 2, 0 };
	thrd_sleep(&delay);
      }
      btnReInit = 1;
    } else {
      btnReInit = 0;
    }
    
    short mValue = get_axis(THROTTLE_AXIS);
    short sValue = get_axis(STEERING_AXIS);
    mValue /= 32;
    mValue += 1024;
    mValue /= 2;
    mValue /= (MAX_SPEED - limit);
    if(mValue > 1024) { mValue = 1024; }
    mValue = get_button(REVERSE_BUTTON) ? -mValue : mValue;

    send_motor(ser, mValue);

    sValue /= 32;
    send_steering(ser, sValue);

    xtime delay = { 0, UPDATE_PERIOD };
    thrd_sleep(&delay);
  }
  
  return 0;
}

int main(int argc, char * argv[]) {
  if (argc <= 2) {
    fprintf(stderr, "  Usage: %s <CONTROLLER> <OUTFILE> [BAUD]\n", argv[0]);
    exit(1);
  }

  int baud = 9600;
  if (argc >= 4) {
    baud = atoi(argv[3]);
  }

  if(strcmp(argv[2], "-") == 0) {
    serial_config.baud = 0;
    serial_config.filename = "/dev/stdout";
    printf("Using stdout\n");
  } else {
    serial_config.baud = baud;
    serial_config.filename = argv[2];
  }  
  
  thrd_t controller_thread, serial_thread;
  int rc_controller, rc_serial;
  thrd_create(&controller_thread, (thrd_start_t) controller, argv[1]);
  thrd_create(&serial_thread, (thrd_start_t) send_data, &serial_config);
  thrd_join(controller_thread, &rc_controller);
  thrd_join(serial_thread, &rc_serial);
  
  return 0;
}
