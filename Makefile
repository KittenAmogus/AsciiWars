SRCDIR = src
BLDDIR = build

# --- Setting ---
CC = gcc
CFLAGS = -Wall -Wextra -I./$(SRCDIR)/core -I./$(SRCDIR)/platform/pc
LDFLAGS = 

MAIN = AsciiWars

# -- Arduino
BOARD = arduino:avr:nano
PORT = /dev/ttyUSB0
SKETCH_DIR = platform/arduino

# -- Sources
CORE_SRCS = $(wildcard $(SRCDIR)/core/*.c)
PC_SRCS   = $(wildcard $(SRCDIR)/platform/pc/*.c)

# -- Objects
CORE_OBJS = $(patsubst $(SRCDIR)/core/%.c, $(BLDDIR)/core/%.o, $(CORE_SRCS))
PC_OBJS   = $(patsubst $(SRCDIR)/platform/pc/%.c, $(BLDDIR)/pc/%.o, $(PC_SRCS))
ALL_OBJS  = $(CORE_OBJS) $(PC_OBJS)

# --- Targets ---
.PHONY: all pc arduino clean upload

all: pc

# Pc (linux)
pc: $(BLDDIR) $(ALL_OBJS)
	@echo "--- Linking for PC -> $(MAIN)"
	$(CC) $(ALL_OBJS) -o $(MAIN) $(LDFLAGS)

# Core
$(BLDDIR)/core/%.o: $(SRCDIR)/core/%.c
	@echo "-- Compiling CORE $< -> $@"
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Pc
$(BLDDIR)/pc/%.o: $(SRCDIR)/platform/pc/%.c
	@echo "-- Compiling PC $< -> $@"
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Arduino
arduino:
	@echo "--- Compiling for $(BOARD)"
	arduino-cli compile --fqbn $(BOARD) $(SKETCH_DIR)

# Arduino flashing
upload:
	@echo "--- Uploading to $(PORT)"
	arduino-cli upload -p $(PORT) --fqbn $(BOARD) $(SKETCH_DIR)

# Cleaning
clean:
	@echo "--- Cleaning up"
	rm -rf $(BLDDIR) $(MAIN)

$(BLDDIR):
	@mkdir -pv $(BLDDIR)

