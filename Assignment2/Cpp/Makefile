# Compiler to use
CXX = g++

# Compiler flags to use
CXXFLAGS = -std=c++11 -Wall

# Target executable to make
TARGET = assignment2

# Source file to use
SRCS = assignment2.cpp

# Object file
OBJS = $(SRCS:.cpp=.o)

# Default rule
all: $(TARGET)

# Rule to create the target executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

# Clean rule to remove compiled files
clean:
	del $(OBJS) $(TARGET).exe
