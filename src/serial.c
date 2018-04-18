#include <errno.h>
#include <fcntl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "command.h"
#include "controller.h"

#define CRLF "\r\n"

#define MOTOR_MIN (-1024)
#define MOTOR_MAX (+1024)

#define STEERING_MIN (-1024)
#define STEERING_MAX (+1024)

void send_initialise(FILE * fp) {
  command_t cmd = { CMD_INIT, 0 };
  command_fprintf(&cmd, fp);
}

void send_motor(FILE * fp, int value) {
  if(value < MOTOR_MIN || value > MOTOR_MAX) {
    fprintf(stderr, "Value %+05d out of range\n", value);
    abort();
  }
  command_t cmd = { CMD_THROTTLE_ABS, value };
  command_fprintf(&cmd, fp);
}

void send_steering(FILE * fp, int value) {
  if(value < MOTOR_MIN || value > MOTOR_MAX) {
    fprintf(stderr, "Value %+05d out of range\n", value);
    abort();
  }
  command_t cmd = { CMD_STEER_ABS, value };
  command_fprintf(&cmd, fp);
}

// Ripped off from stack overflow:
//  https://stackoverflow.com/questions/6947413
int set_interface_attribs (int fd, int speed, int parity) {
  struct termios tty;
  memset (&tty, 0, sizeof tty);
  if (tcgetattr (fd, &tty) != 0) {
    perror("tcgetattr");
    return -1;
  }

  cfsetospeed (&tty, speed);
  cfsetispeed (&tty, speed);

  tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
  // disable IGNBRK for mismatched speed tests; otherwise receive break
  // as \000 chars
  tty.c_iflag &= ~IGNBRK;         // disable break processing
  tty.c_lflag = 0;                // no signaling chars, no echo,
  // no canonical processing
  tty.c_oflag = 0;                // no remapping, no delays
  tty.c_cc[VMIN]  = 0;            // read doesn't block
  tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

  tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

  tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
  // enable reading
  tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
  tty.c_cflag |= parity;
  tty.c_cflag &= ~CSTOPB;
  tty.c_cflag &= ~CRTSCTS;

  if (tcsetattr (fd, TCSANOW, &tty) != 0) {
    perror("tcsetattr");
    return -1;
  }
  
  return 0;
}

void set_blocking (int fd, int should_block) {
  struct termios tty;
  memset(&tty, 0, sizeof tty);
  if (tcgetattr (fd, &tty) != 0) {
    perror("set blocking");
    return;
  }

  tty.c_cc[VMIN]  = should_block ? 1 : 0;
  tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

  if (tcsetattr (fd, TCSANOW, &tty) != 0)
    perror("setting term attributes");
}

FILE * serial_connect(char* port, int baud) {
  int fd = open(port, O_RDWR | O_NOCTTY | O_SYNC);
  if (fd < 0) {
    perror("connecting");
  }

  FILE * fp = fdopen(fd, "r+");
  if(fp == NULL) {
    perror("could not allocate file handle");
  }

  if(baud)
    set_interface_attribs(fileno(fp), baud, 0);

  return fp;
}
