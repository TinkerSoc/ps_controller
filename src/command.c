#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"

char* command_name_parse(command_name_t* name, const char* str) {
  switch(*str) {
  case CMD_BRAKE:
  case CMD_FREEWHEEL:
  case CMD_INIT:
  case CMD_STEER_REL:
  case CMD_STEER_ABS:
  case CMD_THROTTLE_ABS:
    *name = (command_name_t) *(str++);
    break;
  }
  return (char*) str;
}
  
char* command_value_parse(command_value_t* val, const char* orig) {
  const char* str = orig;
  int res = 0;
  int neg;
  switch(*str) {
  case '+':
    neg = 0;
    break;
  case '-':
    neg = 1;
    break;
  default:
    return (char*) orig;
  }

  for (res = 0, str++; COMMAND_VALUE_OK(res); str++) {
    if (*str >= '0' && *str <= '9') {
      res *= 10;
      res += (*str) - '0';
    } else {
      break;
    }
  }

  if(!COMMAND_VALUE_OK(res)) {
    return (char*) orig;
  }

  if (neg) {
    res = -res;
  }
  *val = res;
  return (char*) str;
}

char* command_parse(command_t *cmd, const char *orig) {
  const char* str = orig;
  const char* res;

  res = command_name_parse(&cmd->name, str);
  if (res == str) {
    return (char*) orig;
  }
  str = res;
  
  res = command_value_parse(&cmd->value, str);
  if (res == str) {
    return (char*) orig;
  }
  str = res;

  if (strncmp(str, COMMAND_NEWLINE, strlen(COMMAND_NEWLINE)) == 0) {
    str += strlen(COMMAND_NEWLINE);
    return (char*) str;
  } else { // parsing incomplete. abort
    return (char*) orig;
  }
}
    

int command_snprintf(command_t *cmd, char *s, size_t n) {
  return snprintf(s, n, COMMAND_PRINTF, cmd->name, cmd->value);
}

int command_fprintf(command_t *cmd, FILE *stream) {
  return fprintf(stream, COMMAND_PRINTF, cmd->name, cmd->value);
}
