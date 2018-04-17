#include <stdio.h>
#include <stdlib.h>

#include "controller.h"


int main(int argc, char * argv[]) {
  if (argc <= 1) {
    fprintf(stderr, "  Usage: %s <device file>\n", argv[0]);
    exit(1);
  }

  FILE * fp = initialise(argv[1]);
  printf("Device name: %s\n", device_name);

  update_states(fp);
  return 0;
}
