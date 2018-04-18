# Serial Communication Protocol

The objective of this is to create an extensible protocol for
commanding motors to move over serial.

## Grammar

```
DIGIT := 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
SIGN := + | -

VALUE := SIGN DIGIT+

NL := \r\n

COMMAND := 'I' | 'M'

STMT := COMMAND VALUE NL
```

All lines will be terminated with `CRLF`. An example sequence of commands:


```
I+0
M+300
M-300
```

Initialise, motor forwards 300, motor reverse 300.

## Conclusion
Totes works.
