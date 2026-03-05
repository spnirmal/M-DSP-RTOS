CC := xtensa-esp32s3-elf-gcc
OBJCOPY := xtensa-esp32s3-elf-objcopy
ESPTOOL := esptool.py              # make sure this is in your PATH
PORT    := /dev/ttyUSB0           # adjust for your host (COM3, etc.)
FLASH_MODE := dio
FLASH_FREQ := 80m
FLASH_SIZE := detect

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

TARGET := bin/kernel.elf            # ELF output from linker
BIN    := bin/kernel.raw.bin        # raw binary from objcopy
IMG    := bin/kernel.elf.bin        # ESP image (with header)

all: $(IMG)

$(TARGET): $(OBJ)
	mkdir -p $(dir $@)
	$(CC) $(OBJ) $(LDFLAGS) -o $@

$(BIN): $(TARGET)
	mkdir -p $(dir $@)
	$(OBJCOPY) -O binary $< $@

# elf2image will create $(IMG) next to the ELF; it adds the ESP header
$(IMG): $(TARGET)
	mkdir -p $(dir $@)
	$(ESPTOOL) --chip esp32s3 elf2image $<
	@echo "Generated image: $@"

build/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

build/%.o: %.S
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build bin

# flash the image at the standard application offset (0x1000)
# the ROM will run its internal bootloader which looks here for a valid header
flash: $(IMG)
	$(ESPTOOL) --chip esp32s3 --port $(PORT) \
		--flash_mode $(FLASH_MODE) \
		--flash_freq $(FLASH_FREQ) \
		--flash_size $(FLASH_SIZE) \
		write_flash -z 0x1000 $(IMG)

.PHONY: all clean flash