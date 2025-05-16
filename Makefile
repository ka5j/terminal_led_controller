# Toolchain
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

# Flags
CFLAGS = -mcpu=cortex-m4 -mthumb -Wall -g -O0 -ffreestanding -nostdlib
LDFLAGS = -TSTM32F446RETX_FLASH.ld -nostdlib
ASFLAGS = -mcpu=cortex-m4 -mthumb

# Directories
SRC_DIR = src
INC_DIR = inc
STARTUP_DIR = startup
BUILD_DIR = build

# Source files
C_SOURCES := $(wildcard $(SRC_DIR)/*.c)
OBJECTS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(C_SOURCES)) $(BUILD_DIR)/startup.o
INCLUDES = -I$(INC_DIR)

# Target
TARGET = $(BUILD_DIR)/main.elf

# Rules
all: $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/startup.o: $(STARTUP_DIR)/startup_stm32f446retx.s | $(BUILD_DIR)
	$(CC) $(ASFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	$(SIZE) $@

clean:
	rm -rf $(BUILD_DIR)

flash:
	openocd -f interface/stlink.cfg -f target/stm32f4x.cfg \
	-c "program build/main.elf verify reset; resume; exit" || true


