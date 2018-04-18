#ifndef __CONTROLLER_H_
#define __CONTROLLER_H_
#include <stdio.h>

char * device_name;

FILE * initialise(char * filename);
void update_states(FILE * fp);

short get_axis(int axis);
short get_button(int axis);

#endif // __CONTROLLER_H_
