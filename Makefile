
# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -pthread

# Target executable
TARGET = student_app

# Source files
SRCS = main.c Student.c Thread.c

# Object files
OBJS = $(SRCS:.c=.o)

# Header files
HEADERS = Student.h Thread.h

# Default target
all: $(TARGET)

# Rule to build the target executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to build object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target to remove generated files
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean


