#avrdude -c avrispv2 -p m16 -P /dev/ttyACM0 -U lfuse:w:0xEF:m
#avrdude -c avrispv2 -p m16 -P /dev/ttyACM0 -U hfuse:w:0xC9:m
avr-gcc -mmcu=atmega16 -DF_CPU=16000000UL -Os -o R2R.o R2R.c && \
avr-objcopy -O ihex R2R.o R2R.hex && \
avrdude -c avrispv2 -p m16 -P /dev/ttyACM0 -U flash:w:R2R.hex
