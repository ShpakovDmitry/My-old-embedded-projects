#!/bin/bash
avr-gcc -mmcu=attiny2313 -DF_CPU=20000000UL -Os -o cube.o main.c && \
avr-objcopy -O ihex cube.o cube.hex && \
avrdude -c avrispv2 -p attiny2313 -P /dev/ttyACM0 -U flash:w:cube.hex

