# ps_controller

Control software for a heavily customised _Shoprider Sovreign 4_ mobility scooter.

## Controller

Translates linux joystick inputs into serial commands for the arduino.

```bash
make controller
./controller /dev/input/js0 /dev/ttyACM0 115200
```

## Arduino

Translates serial commands into hardware inputs for motor controllers.

```bash
export ARDUINO_DIR=$HOME/.arduino/arduino-1.8.5 # arduino IDE directory
export ARDUINO_MK=$HOME/src/Arduino-Makefile # Arduino-Makefile directory
export
make -C arduino upload
```

## Missing Functionality

- Decent name
