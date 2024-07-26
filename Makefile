# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -pthread

# Target executable
TARGET = student_app

# Source files
SRCS = main.c student.c teacher.c thread.c log.c login.c

# Object files
OBJS = $(SRCS:.c=.o)

# Header files
HEADERS = student.h teacher.h thread.h log.h login.h

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
