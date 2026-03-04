CC := xtensa-esp32s3-elf-gcc
OBJCOPY := xtensa-esp32s3-elf-objcopy

CFLAGS := -Wall -Wextra -Os -ffreestanding -nostdlib
CFLAGS += -mlongcalls

LDFLAGS := -T linker.ld -nostdlib

SRCDIR := src
OBJDIR := build
BINDIR := bin

SRC := $(wildcard $(SRCDIR)/*.c)
SRC += $(wildcard $(SRCDIR)/*.S)

OBJ := $(patsubst $(SRCDIR)/%, $(OBJDIR)/%, $(SRC:.c=.o))
OBJ := $(OBJ:.S=.o)

TARGET := $(BINDIR)/kernel.elf
BIN := $(BINDIR)/kernel.bin

all: $(BIN)

$(TARGET): $(OBJ)
	mkdir -p $(BINDIR)
	$(CC) $(OBJ) $(LDFLAGS) -o $@

$(BIN): $(TARGET)
	$(OBJCOPY) -O binary $< $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.S
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(BINDIR)

flash:
	esptool.py --chip esp32s3 --port /dev/ttyUSB0 write_flash 0x0 $(BIN)

.PHONY: all clean flash