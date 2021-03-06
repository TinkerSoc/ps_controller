#include <stdio.h>

#ifndef COMMAND_H
#define COMMAND_H

#define COMMAND_VALUE_HIGH (1024)
#define COMMAND_VALUE_LOW (-1024)
#define COMMAND_VALUE_OK(n) (n >= COMMAND_VALUE_LOW && n <= COMMAND_VALUE_HIGH)

#define COMMAND_NEWLINE "\r\n"
#define COMMAND_PRINTF ("%c%+d" COMMAND_NEWLINE)

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
extern "C" {
#endif

typedef enum {
  CMD_BRAKE = 'B', // Set brake
  CMD_FREEWHEEL = 'F', // Motor controller Freewheel 
  CMD_INIT = 'I', // Initialisation command
  CMD_STEER_REL = 'R', // Relative steering
  CMD_STEER_ABS = 'S', // Absolute steering
  CMD_THROTTLE_ABS = 'T', // Absolute throttle
} command_name_t;

typedef int command_value_t;

typedef struct command_t {
  command_name_t name;
  command_value_t value;
} command_t;

/*
 * Parse a command name. See `command_parse` for return
 * values and side effects.
 */
char* command_name_parse(command_name_t*, const char*);

/*  
 * Parse a command value. See `command_parse` for return
 * values and side effects.
 */
char* command_value_parse(command_value_t*, const char*);

    
/*
 * Parse a string into a command struct.
 * Returns a pointer to the end of the consumed input.
 * Does not consume any input on parse failure.
 * Struct may contain garbage if parsing fails.
 * For example:
 *
 *     command_t cmd;
 *     char *raw = "M+150\r\n";
 *     
 *     char* res = command_parse(&cmd, raw);
 *
 *     if ( res == raw ) { // parse failure }
 */
char* command_parse(command_t*, const char*);

/*
 * Print a command to a file handle.
 *
 * See `fprintf` for return values and errno.
 *
 * For example:
 *
 *     command cmd;
 *     cmd.name = CMD_MOVE;
 *     cmd.val = 100;
 *
 *     command_fprintf(*cmd, stdout);
 */
int command_fprintf(command_t*, FILE*);

/*
 * Prints a command to a string.
 * See `snprintf` for return values and errno.
 * Example:
 *
 *     command_t cmd;
 *     cmd.name = CMD_MOVE;
 *     cmd.val = 100;
 *     
 *     char buffer[100];
 *
 *     command_snprintf(&cmd, buffer, 100);
 */
int command_snprintf(command_t*, char*, size_t);

#ifdef __cplusplus /* If this is a C++ compiler, end C linkage */
}
#endif

#endif // COMMAND_H
