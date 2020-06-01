CC = avr-gcc
CCFLAGS += -mmcu=$(MMCU) -DF_CPU=$(F_CPU) $(OPTIMISATION) -Wall -Iinclude -I../include 
LDFLAGS +=  -Wl,-Map,$(BUILD_DIR)/$(TARGET).map
BUILD_DIR = build
SOURCE_DIR = source
SOURCES = $(wildcard $(SOURCE_DIR)/*.c)
OBJECTS = $(SOURCES:$(SOURCE_DIR)/%.c=$(BUILD_DIR)/%.o)

all: $(BUILD_DIR) $(BUILD_DIR)/$(TARGET).hex

$(BUILD_DIR):
	mkdir --parents $(BUILD_DIR)

$(BUILD_DIR)/$(TARGET).hex: $(BUILD_DIR)/$(TARGET).o
	avr-objcopy -O ihex $< $@

$(BUILD_DIR)/$(TARGET).o: $(OBJECTS)
	$(CC) $(CCFLAGS) $(LDFLAGS) $(OBJECTS) -o $@ 
	
$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c
	$(CC) $(CCFLAGS) -c $< -o $@
	
clean:
	rm -rf $(BUILD_DIR)
