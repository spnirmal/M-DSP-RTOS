CC := xtensa-esp32s3-elf-gcc
OBJCOPY := xtensa-esp32s3-elf-objcopy

# compiler flags
CFLAGS := -Wall -Wextra -Os -ffreestanding -nostdlib -mlongcalls
CFLAGS += -Iinclude          # project include directory

LDFLAGS := -T linker.ld -nostdlib

# source files used in the firmware
# top-level C/ASM, RTOS core, port layer, etc.
SRC := main.c interrupt.c startup.S vectors.S \
       $(wildcard src/*.c) $(wildcard src/*.S) \
       $(wildcard port/esp32s3/*.c) $(wildcard port/esp32s3/*.S)

OBJ := $(patsubst %.c,build/%.o,$(SRC))
OBJ := $(patsubst %.S,build/%.o,$(OBJ))

TARGET := bin/kernel.elf
BIN    := bin/kernel.bin

all: $(BIN)

$(TARGET): $(OBJ)
	mkdir -p $(dir $@)
	$(CC) $(OBJ) $(LDFLAGS) -o $@

$(BIN): $(TARGET)
	$(OBJCOPY) -O binary $< $@

build/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

build/%.o: %.S
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build bin

flash:
	esptool.py --chip esp32s3 --port /dev/ttyUSB0 write_flash 0x0 $(BIN)

.PHONY: all clean flash