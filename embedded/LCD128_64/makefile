LCD.hex: LCD.c LCD.o
	avr-gcc -mmcu=atmega16 -DF_CPU=12000000UL -Os -o LCD.o LCD.c && \
  avr-objcopy -O ihex LCD.o LCD.hex
clean:
	rm -rf *.hex *.o
