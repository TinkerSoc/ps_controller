#ifndef __SERIAL_H_
#define __SERIAL_H_

void send_initialise(FILE * fp);
void send_motor(FILE * fp, int value);

FILE * serial_connect(char * port, int baud);

#endif // __SERIAL_H_
