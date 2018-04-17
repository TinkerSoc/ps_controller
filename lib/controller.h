#ifndef __CONTROLLER_H_
#define __CONTROLLER_H_
#include <stdio.h>

char * device_name;

FILE * initialise(char * filename);
void update_states(FILE * fp);

#endif // __CONTROLLER_H_
