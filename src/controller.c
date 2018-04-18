#include <inttypes.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>

#include <linux/input.h>
#include <linux/joystick.h>

#define AXIS_MAP_SIZE (ABS_CNT)
#define BUTTON_MAP_SIZE (KEY_MAX - BTN_MISC + 1)

 int fileno(FILE* fp);

typedef struct joystick_data {
  int time;
  short value;
  unsigned char type;
  unsigned char number;
} joystick_data_t;
 
typedef enum button_name {
  THUMB = 0x121,
  
  DPAD_UP = 0x124,
  DPAD_RIGHT = 0x125,
  DPAD_DOWN = 0x126,
  DPAD_LEFT = 0x127,
  THROTTLE = 0x128,
  
  BUMPER = 0x12A,
  BTNO = 0x12D,
  BTNX = 0x12E,
  
  BTN_PS = 0x2C0  
} button_name_t;

typedef enum axis_name {
  AXIS_X = 0x00,
  AXIS_Y = 0x01,

  AXIS_THROTTLE_A = 0x2C,
  AXIS_DPAD_UP_A = 0x28,
  AXIS_DPAD_RIGHT_A = 0x29,
  AXIS_DPAD_DOWN_A = 0x2A,
  AXIS_DPAD_LEFT_A = 0x2B,
  AXIS_BUMPER_A = 0x2D
} axis_name_t;

char * device_name;
uint8_t axis_map[AXIS_MAP_SIZE];
uint16_t button_map[BUTTON_MAP_SIZE];
short * states;

unsigned int num_axis;
unsigned int num_buttons;

FILE * initialise(char * filename) {
  FILE * fp = fopen(filename, "rb");

  // TODO: better error printing is needed here.
  if(fp == NULL) {
    fprintf(stderr, "Probably no such file (%s)\n", filename);
    exit(1);
  }

  if(device_name != NULL)
    free(device_name);
  device_name = (char*) calloc(64, sizeof(char));
  ioctl(fileno(fp), JSIOCGNAME(64), device_name);
  
  ioctl(fileno(fp), JSIOCGAXES, &num_axis);
  printf("Number of axes: %d\n", num_axis);
  
  ioctl(fileno(fp), JSIOCGBUTTONS, &num_buttons);
  printf("Number of buttons: %d\n", num_buttons);

  ioctl(fileno(fp), JSIOCGAXMAP, axis_map);
  ioctl(fileno(fp), JSIOCGBTNMAP, button_map);

  states = calloc(num_buttons+num_axis, sizeof(short));
  
  for(int i = 0; i < AXIS_MAP_SIZE; i++) {
    if(axis_map[i])
      printf("[%02d] -> 0x%02x\n", i, axis_map[i]);
  }
  
  return fp;
}

void update_states(FILE * fp) {
  struct js_event data;
  fread(&data, sizeof(data), 1, fp);

  // Button events
  if(data.type & JS_EVENT_BUTTON) {
    if(data.number < num_buttons && states != NULL) {
      states[data.number] = data.value;
    }
  }

  if(data.type & JS_EVENT_AXIS) {
    printf("Joystick event\n");
    printf("Axis number: %d\n", data.number);
    printf("Axis value: %d\n", data.value);
    if(data.number < num_axis && states != NULL) {
      states[num_buttons + data.number] = data.value;
    }
  }
}

short get_button(size_t button) {
  if(button >= num_buttons) {
    fprintf(stderr, "Error: Button %lu is out of range of this %d button device.\n", button, num_buttons);
    return 0;
  }
  if(states == NULL) {
    return 0;
  }
  
  return states[button];
}

short get_axis(size_t axis) {
  if(axis >= num_axis) {
    fprintf(stderr, "Error: Axis %lu is out of range of this %d axis device.\n", axis, num_axis);
    return 0;
  }
  if(states == NULL) {
    return 0;
  }
  
  return states[num_buttons + axis];
}
