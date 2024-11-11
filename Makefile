# Compiler
CC := g++

# Compiler flags
CFLAGS := -Wall -Wextra -O2

# Include and library paths
INCLUDES := -I$(HOME)/raylib/src
LDFLAGS := -L/usr/local/lib

# Libraries to link against
LIBS := -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Source files
SRCS := minswepper.cpp

# Object files
OBJS := $(SRCS:.cpp=.o)

# Executable name
TARGET := minswepper

# Default target
all: $(TARGET)

# Linking
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS) $(LIBS)

# Compilation
%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean