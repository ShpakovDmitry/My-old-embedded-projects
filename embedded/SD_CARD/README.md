Source for operations with SD or MMC card using SPI interface.

Source is only for my educational and curiosity purposes, to investigate how to work with such kind of memory storage devices.

It is were supposed that uC board is connected to PC via FT232RL chip, which leads uC to communicate with PC via USART interface.

PC is used for debug purposes to view which commands are sent to memory card and what answer is going out from memory card.

I succed to implement routines to communicate with memory card usind SD card standard datasheet,
but communication was very unstable.

Project and thus library is not finished.

# *comport.c*
 Is source file for PC side handling communication with uC using terminal. Log example is in file *data.txt*
# *sd_card.h*
 Is supposed to be library for memory card comunnications
# *testcard.c*
 Source file for communication test and debug purposes at uC side.


To build project following tools should be installed:
```
avr-gcc, avr-binutils, avr-libc
```
To get them run following commands
### On Debian

```
$ apt-get update
$ apt-get install avr-gcc avr-binutils avr-libc
```
