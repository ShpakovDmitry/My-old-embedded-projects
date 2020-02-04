Project to test LCD functions.

LCD is 128x64 pixels wide. Supports TEXT and GRAPHICS mode.
Control routines are made using LCD's datasheet.

Program counts realtime seconds based on system clock, converts it to hh:mm:ss format and prints it on the LCD.

To build project following tools should be installed:
```
avr-gcc, avr-binutils, avr-libc
```
To get them run following commands
### On Linux

```
$ apt-get update
$ apt-get install avr-gcc avr-binutils avr-libc
```
