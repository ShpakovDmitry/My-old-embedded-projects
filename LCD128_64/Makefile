CC = avr-gcc
CCFLAGS += -mmcu=atmega16 -DF_CPU=12000000UL -Os 
BUILD_DIR = build

all: $(BUILD_DIR) $(BUILD_DIR)/LCD.hex

$(BUILD_DIR):
	mkdir --parents $(BUILD_DIR)
	
$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.o
	avr-objcopy -O ihex $< $@

$(BUILD_DIR)/%.o: %.c
	$(CC) $(CCFLAGS) -o $@ $<

clean:
	rm -rf $(BUILD_DIR)
