med_panel.hex: med_panel.c med_panel.o
  avr-gcc -mmcu=attiny2313 -DF_CPU=16000000UL -Os -o med_panel.o med_panel.c && \
  avr-objcopy -O ihex med_panel.o med_panel.hex
radio_morse_panel.hex: radio_morse_panel.c radio_morse_panel.o
  avr-gcc -mmcu=attiny2313 -DF_CPU=16000000UL -Os -o radio_morse_panel.o radio_morse_panel.c && \
  avr-objcopy -O ihex radio_morse_panel.o radio_morse_panel.hex
