#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"


char* command_parse(command_t *cmd, char *s) {
  command_name_t name;
  command_val_t val;
  char *orig = s;
  if (strlen(s) < 2) {
    return s;
  }
  
  switch(s[0]) {
  case CMD_INIT:  // init
  case CMD_MOVE: // absolute move
  case CMD_RELMOVE: // relative move
    name = s[0];
    s++;
    break;
  default:
    return s;
  }

  // char* eptr;
  val = strtol(s, &s, 10);

  if (strncmp(s, "\r\n", 2) == 0) {
    s += 2;
    cmd->name = name;
    cmd->val = val;
    return s;
  } else { // parsing incomplete. abort
    return orig;
  }

} 

int command_snprintf(command_t *cmd, char *s, size_t n) {
  return snprintf(s, n, COMMAND_PRINTF, cmd->name, cmd->val);
}

int command_fprintf(command_t *cmd, FILE *stream) {
  return fprintf(stream, COMMAND_PRINTF, cmd->name, cmd->val);
}
