#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>

#include <linux/joystick.h>

#define AXIS_MAP_SIZE (0x2000)
#define BUTTON_MAP_SIZE (0x2000)

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
char * axis_map;
char * button_map;

int num_axis;
int num_buttons;

FILE * initialise(char * filename) {
  FILE * fp = fopen(filename, "rb");

  if(device_name != NULL)
    free(device_name);
  device_name = (char*) calloc(64, sizeof(char));
  ioctl(fileno(fp), JSIOCGNAME(64), device_name);
  
  ioctl(fileno(fp), JSIOCGAXES, &num_axis);
  printf("Number of axes: %d\n", num_axis);
  
  ioctl(fileno(fp), JSIOCGBUTTONS, &num_buttons);
  printf("Number of buttons: %d\n", num_buttons);

  if(axis_map != NULL)
    free(axis_map);
  axis_map = (char*) calloc(AXIS_MAP_SIZE, sizeof(char));
  ioctl(fileno(fp), JSIOCGAXMAP, axis_map);

  if(button_map != NULL)
    free(button_map);
  button_map = (char*) calloc(BUTTON_MAP_SIZE, sizeof(char));
  ioctl(fileno(fp), JSIOCGBTNMAP, button_map);
  
  return fp;
}

void update_states(FILE * fp) {
  if(!num_axis)
    return;

  int elems = 20;
  joystick_data_t * data = calloc(elems, sizeof(joystick_data_t));
  // assert(sizeof(joystick_data_t)==8);
  fread(data, sizeof(data), elems, fp);

  for(int i = 0; i<elems; i++) {
    printf("Time: %d\n", data[i].time);
    printf("Value: %hd\n", data[i].value);
    printf("Type: 0x%02hhx\n", data[i].type);
    printf("Number: 0x%02hhx\n\n", data[i].number);
  }
}
