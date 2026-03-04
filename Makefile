# PC-Makefile, for Arduino will be later

# Utils
CC = gcc

# Directories
SRCDIR = src
BUILDDIR = build

# C
CC_FLAGS = -O2 -Wall
CC_SOURCES = $(shell find $(SRCDIR) -name '*.c')
CC_OBJECTS = $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%.o,$(CC_SOURCES))

# Directories in src
INNER_DIRS_SRC = $(shell find $(SRCDIR) -mindepth 1 -type d)
INNER_DIRS_BUILD = $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(INNER_DIRS_SRC))

# Binary file
MAINFILE = $(BUILDDIR)/AsciiWars


# -- Functions

# Complete
.PHONY: all clean build link run

# Just make
all: $(BUILDDIR) build link


# Compile to objects
build: $(CC_OBJECTS) $(BUILDDIR)/
	@echo "-- Build completed"

# Link to binary
link: build
	@echo "-- Linking"
	$(CC) -o $(MAINFILE) $(CC_OBJECTS)

# Run binary
run: link
	@echo "-- Running $(MAINFILE)"
	./$(MAINFILE)

# Remove all compiled
clean:
	@echo "-- Cleaning up"
	@rm -rf $(BUILDDIR)

# Compile C
$(BUILDDIR)/%.c.o: $(SRCDIR)/%.c | $(BUILDDIR)/
	@echo "-- Compiling $< -> $@"
	$(CC) -c $< -o $@ -I$(SRCDIR)

# Create dirs
$(BUILDDIR)/:
	@mkdir -pv $@ $(INNER_DIRS_BUILD)

