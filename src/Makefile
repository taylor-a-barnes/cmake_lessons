# Compiler and flags
CXX := g++
CXXFLAGS := -Wall -Wextra -O2
TARGET := helloworld
SRC := main.cpp
OBJ := $(SRC:.cpp=.o)

# Default target
all: $(TARGET)

# Linking
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compilation
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJ) $(TARGET)

# Phony targets
.PHONY: all clean
