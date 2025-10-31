CC := gcc
CFLAGS := -Wall -Wextra -g -std=gnu11
LDFLAGS :=

# Directories
SRCDIR := src
INCDIR := include
OBJDIR := build
BINDIR := bin

# Automatically include all subdirectories under include/
CFLAGS += $(addprefix -I,$(shell find $(INCDIR) -type d))

# Source and object files
SRC := $(shell find $(SRCDIR) -name '*.c')
OBJ := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC))

TARGET := $(BINDIR)/mdsp_rtos

# Default target
all: dirs $(TARGET)

# Create build and bin directories
dirs:
	@mkdir -p $(OBJDIR) $(BINDIR)

# Compile each .c file into a corresponding .o file (keeping directory structure)
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Link all objects
$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $@

# Clean build and binary files
clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all clean dirs
