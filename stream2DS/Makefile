DEVKITPRO ?= $(shell echo $$DEVKITPRO)
DEVKITARM ?= $(shell echo $$DEVKITARM)

CC := $(DEVKITARM)/bin/arm-none-eabi-gcc
CXX := $(DEVKITARM)/bin/arm-none-eabi-g++
OBJCOPY := $(DEVKITARM)/bin/arm-none-eabi-objcopy

TARGET := stream2ds
BUILD := build
SOURCE := source

CFLAGS := -O2 -Wall -mfloat-abi=softfp -march=armv6k -mtune=mpcore -mthumb -ftls-model=local-exec \
          -I$(DEVKITPRO)/libctru/include
LDFLAGS := -L$(DEVKITPRO)/libctru/lib -lctru -lm

ELF := $(BUILD)/$(TARGET).elf
3DSX := $(BUILD)/$(TARGET).3dsx
OBJECTS := $(BUILD)/main.o

.PHONY: all clean

all: $(3DSX)

$(BUILD):
	@mkdir -p $(BUILD)

$(BUILD)/%.o: $(SOURCE)/%.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(ELF): $(OBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(3DSX): $(ELF)
	3dsxtool $< $@ --smdh=$(TARGET).smdh

clean:
	@rm -rf $(BUILD)
