CC := gcc
CFLAGS := -Wall -Wextra -g -std=gnu11
LDFLAGS :=

PORT ?= posix

SRCDIR := src
PORTDIR := port/$(PORT)
INCDIR := include
OBJDIR := build
BINDIR := bin

CFLAGS += -I$(INCDIR)
CFLAGS += -I$(PORTDIR)

SRC := $(wildcard $(SRCDIR)/*.c)
SRC += $(wildcard $(PORTDIR)/*.c)

OBJ := $(patsubst %.c,$(OBJDIR)/%.o,$(SRC))

TARGET := $(BINDIR)/mdsp_rtos

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p $(BINDIR)
	$(CC) $(OBJ) -o $@

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all clean
