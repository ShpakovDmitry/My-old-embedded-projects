#!/bin/bash

avr-as main.s -mmcu=attiny13a -o main && \
avr-ld -o main.elf main && \
avr-objcopy -O ihex main.elf main.hex && \
avrdude -c avrispv2 -p m16 -P /dev/ttyACM0 -U flash:w:main.hex
