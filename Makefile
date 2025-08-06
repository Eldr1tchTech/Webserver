# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g -O2
LDFLAGS = 

# Directories
SRCDIR = src
OBJDIR = obj
BINDIR = bin

# Target executable name
TARGET = webserver

# Find all .c files in src directory
SOURCES = $(wildcard $(SRCDIR)/*.c)
# Convert .c files to .o files in obj directory
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Default target
all: $(BINDIR)/$(TARGET)

# Link object files to create executable
$(BINDIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CC) $(LDFLAGS) $(OBJECTS) -lm -o $@
	@echo "Build complete: $@"

# Compile .c files to .o files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -rf $(OBJDIR) $(BINDIR)

# Run the server
run: $(BINDIR)/$(TARGET)
	./$(BINDIR)/$(TARGET)

# Rebuild everything
rebuild: clean all

.PHONY: all clean run rebuild