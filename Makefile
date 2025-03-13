# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g

# Output executable
TARGET = main

# Source and object files
SRC = main.cpp Partition.cpp find_no_solutions.cpp utils.cpp PartitionGenerator.cpp
OBJ = $(SRC:.cpp=.o)

# Header files
HEADERS = Partition.h find_no_solutions.h utils.h PartitionGenerator.h

# Default rule
all: $(TARGET)

# Link object files to create the final executable
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET)

# Rule to compile .cpp files into .o files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean object and executable files
clean:
	rm -f $(OBJ) $(TARGET)

# Ignore certain files in Git
gitignore:
	echo "*.o" >> .gitignore
	echo "$(TARGET)" >> .gitignore

.PHONY: clean gitignore
