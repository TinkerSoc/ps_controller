#include <inttypes.h>
#include <unistd.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>

#include <linux/input.h>
#include <linux/joystick.h>

#define AXIS_MAP_SIZE (ABS_CNT)
#define BUTTON_MAP_SIZE (KEY_MAX - BTN_MISC + 1)

int fileno(FILE* fp);

char * device_name;
uint8_t axis_map[AXIS_MAP_SIZE];
uint16_t button_map[BUTTON_MAP_SIZE];
short * states;

unsigned int num_axis;
unsigned int num_buttons;

volatile atomic_int states_lock;

FILE * initialise(char * filename) {
  FILE * fp = fopen(filename, "rb");

  // TODO: better error printing is needed here.
  if(fp == NULL) {
    fprintf(stderr, "Probably no such file (%s)\n", filename);
    exit(1);
  }

  int fd = fileno(fp);
  
  if(device_name != NULL)
    free(device_name);

  device_name = (char*) calloc(64, sizeof(char));
  ioctl(fd, JSIOCGNAME(64), device_name);
  
  ioctl(fd, JSIOCGAXES, &num_axis);  
  ioctl(fd, JSIOCGBUTTONS, &num_buttons);
  ioctl(fd, JSIOCGAXMAP, axis_map);
  ioctl(fd, JSIOCGBTNMAP, button_map);

  states = calloc(num_buttons+num_axis, sizeof(short));
  
  return fp;
}

int get_lock() {
  int exp = 0;
  int rc = 0;
  while(!rc) {
    rc = atomic_compare_exchange_weak(&states_lock, &exp, 1);
  }
  return rc;
}

void release_lock() {
  atomic_store_explicit(&states_lock, 0, memory_order_release);
}

void update_states(FILE * fp) {
  get_lock();
  
  struct js_event data;
  fread(&data, sizeof(data), 1, fp);

  // Button events
  if(data.type & JS_EVENT_BUTTON) {
    if(data.number < num_buttons && states != NULL) {
      int idx = button_map[data.number];
      states[idx] = data.value;
    }
  }

  if(data.type & JS_EVENT_AXIS) {
    if(data.number < num_axis && states != NULL) {
      int idx = axis_map[data.number];
      states[idx + num_buttons] = data.value;
    }
  }

  release_lock();
}

short get_button(size_t button) {
  if(states == NULL) {
    return 0;
  }

  get_lock();
  short ret = states[button];
  release_lock();
  
  return ret;
}

short get_axis(size_t axis) {
  if(states == NULL) {
    return 0;
  }

  get_lock();
  short ret = states[num_buttons + axis];
  release_lock();
  
  return ret;
}
