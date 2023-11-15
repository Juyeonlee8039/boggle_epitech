# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra

# Target executable name
TARGET = main

# Source files
SRCS = $(wildcard src/*.c)

# Object files
OBJS = $(SRCS:.c=.o)

# Phony targets
.PHONY: all clean fclean re

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Pattern rule for compiling source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean object files
clean:
	rm -f $(OBJS)

# Clean everything, including the executable
fclean: clean
	rm -f $(TARGET)

# Clean everything and rebuild
re: fclean all
