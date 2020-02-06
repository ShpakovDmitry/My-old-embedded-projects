MMCU = atmega16
F_CPU = 12000000UL
OPTIMISATION = -Os

CC = avr-gcc
CCFLAGS += -mmcu=$(MMCU) -DF_CPU=$(F_CPU) $(OPTIMISATION)
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
