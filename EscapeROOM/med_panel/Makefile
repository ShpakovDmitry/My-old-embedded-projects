MMCU = attiny2313
F_CPU = 16000000UL
OPTIMISATION = -Os
TARGET = med_panel

CC = avr-gcc
CCFLAGS += -mmcu=$(MMCU) -DF_CPU=$(F_CPU) $(OPTIMISATION) 
BUILD_DIR = build

all: $(BUILD_DIR) $(BUILD_DIR)/$(TARGET).hex

$(BUILD_DIR):
	mkdir --parents $(BUILD_DIR)

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.o
	avr-objcopy -O ihex $< $@
	
$(BUILD_DIR)/%.o: %.c
	$(CC) $(CCFLAGS) -o $@ $<
	
clean:
	rm -rf $(BUILD_DIR)
