CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g

TARGET = search_no_solution

SHLOMO_DIR = shlomo_src
SRC_FILES = main.cpp find_no_solutions.cpp utils.cpp
SRC_DIR_FILES = $(wildcard $(SHLOMO_DIR)/*.cpp)

SRC = $(SRC_FILES) $(SRC_DIR_FILES)

OBJ = $(SRC:.cpp=.o)

HEADERS_DIR_FILES = $(wildcard $(SHLOMO_DIR)/*.h)
HEADERS = find_no_solutions.h utils.h
HEADERS += $(HEADERS_DIR_FILES)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

gitignore:
	echo "*.o" >> .gitignore
	echo "$(TARGET)" >> .gitignore

.PHONY: clean gitignore
