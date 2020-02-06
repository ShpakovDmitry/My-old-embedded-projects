CC = avr-gcc
CCFLAGS += -mmcu=attiny2313 -DF_CPU=16000000UL -Os 
BUILD_DIR = build

all: $(BUILD_DIR) $(BUILD_DIR)/med_panel.hex $(BUILD_DIR)/radio_morse_panel.hex

$(BUILD_DIR):
	mkdir --parents $(BUILD_DIR)

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.o
	avr-objcopy -O ihex $< $@
	
$(BUILD_DIR)/%.o: %.c
	$(CC) $(CCFLAGS) -o $@ $<
	
clean:
	rm -rf $(BUILD_DIR)
