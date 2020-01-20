test2.hex: test2.o
	avr-objcopy -O ihex test2.o test2.hex
	avrdude -c avrispv2 -p m16 -P /dev/ttyACM0 -U flash:w:test2.hex
test2.o: test2.c
	avr-gcc -mmcu=atmega16 -DF_CPU=12000000UL -Os -o test2.o test2.c


#avr-gcc -mmcu=atmega16 -DF_CPU=12000000UL -Os -o test2.o test2.c
#avr-objcopy -O ihex test2.o test2.hex
#avrdude -c avrispv2 -p m16 -P /dev/ttyACM0 -U flash:w:test2.hex
