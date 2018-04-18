#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"


char* command_parse(command_t *cmd, char *s) {
  command_name_t name;
  command_value_t value;
  char *orig = s;
  if (strlen(s) < 2) {
    return s;
  }
  
  switch(s[0]) {
  case CMD_INIT:
  case CMD_STEER_REL:
  case CMD_STEER_ABS:
  case CMD_THROTTLE_ABS:
    name = (command_name_t) s[0];
    s++;
    break;
  default:
    return s;
  }

  value = strtol(s, &s, 10);

  if (strncmp(s, COMMAND_NEWLINE, strlen(COMMAND_NEWLINE)) == 0) {
    s += strlen(COMMAND_NEWLINE);
    cmd->name = name;
    cmd->value = value;
    return s;
  } else { // parsing incomplete. abort
    return orig;
  }
}
    

int command_snprintf(command_t *cmd, char *s, size_t n) {
  return snprintf(s, n, COMMAND_PRINTF, cmd->name, cmd->value);
}

int command_fprintf(command_t *cmd, FILE *stream) {
  return fprintf(stream, COMMAND_PRINTF, cmd->name, cmd->value);
}
