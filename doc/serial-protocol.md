# Serial Communication Protocol

The objective of this is to create an extensible protocol for
commanding motors to move over serial.

## Grammar

```
DIGIT := 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
SIGN := + | -

VALUE := SIGN DIGIT+

NL := '\r' '\n'

COMMAND := 'B' | 'F' | 'I' | 'R' | 'S' | 'T'

STMT := COMMAND VALUE NL
```

## Commands

Regarding interpretation of numbers, `-0` and `+0` are assumed to have the same
value.

### Braking (`B`)

Set axle brake.

| Value | Description |
|-------|-------------|
| `+0`  | Disable     |
| `+1`  | Enable      |

### Freewheeling (`F`)

Allow coasting.

| Value | Description |
|-------|-------------|
| `+0`  | Disable     |
| `+1`  | Enable      |

### Initialisation (`I`)

Calibrate steering and enable motors.

| Value | Description                               |
|-------|-------------------------------------------|
| `+0`  | Keep steering calibration, disable motors |
| `+1`  | Calibrate steering, enable motors         |

### Relative Steering (`R`)

Steer by a specified amount from the current position.

| Value           | Steering Direction |
|-----------------|--------------------|
| `-1024` to `-1` | Left               |
| `-0` and `+0`   | None               |
| `+1` to `+1024` | Right              |


## Absolute Steering (`S`)

Set the steering to the provided absolute position.

| Value           | Description          |
|-----------------|----------------------|
| `-1024` to `-0` | Full left to centre  |
| `+0` to `+1024` | Centre to full right |

## Absolute Throttle (`T`)

| Value           | Description          |
|-----------------|----------------------|
| `-1024` to `-0` | Full reverse to idle |
| `+0` to `+1024` | Idle to full ahead   |
