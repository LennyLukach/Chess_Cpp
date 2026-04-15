CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -pedantic

# SDL2 flags
SDL2_CFLAGS := $(shell sdl2-config --cflags)
SDL2_LIBS := $(shell sdl2-config --libs) -lSDL2_image

SRC_DIR := src
# Find all .cpp files recursively in src and subdirectories
SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
OBJS := $(SRCS:.cpp=.o)

TARGET := main

.PHONY: all run debug clean

all: clean $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(SDL2_LIBS)

# Pattern rule that works for any subdirectory depth
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(SDL2_CFLAGS) -c $< -o $@

run: all
	./$(TARGET)

debug: CXXFLAGS := -std=c++17 -g -O0 -Wall -Wextra -pedantic
debug: clean all

clean:
	rm -f $(OBJS) $(TARGET)