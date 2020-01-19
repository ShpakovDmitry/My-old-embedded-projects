#!/bin/bash
#avr-gcc -S -mmcu=atmega16 -DF_CPU=16000000UL -Os sensor.c && \
avrdude -c avrispv2 -p m16 -P /dev/ttyACM0 -U lfuse:w:0xEF:m
avrdude -c avrispv2 -p m16 -P /dev/ttyACM0 -U hfuse:w:0xC9:m
avr-gcc -mmcu=atmega16 -DF_CPU=16000000UL -Os -o radio_master.o radio_master.c && \
avr-objcopy -O ihex radio_master.o radio_master.hex && \
avrdude -c avrispv2 -p m16 -P /dev/ttyACM0 -U flash:w:radio_master.hex
