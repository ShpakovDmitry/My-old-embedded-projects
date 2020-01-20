#!/bin/bash

avr-as main.s -mmcu=atmega16 -o main && \
avr-ld -o main.elf main && \
avr-objcopy -O ihex main.elf main.hex
