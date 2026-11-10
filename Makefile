CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Wpedantic -O2
LDFLAGS = 

SRCS = main.cpp tests.cpp benchmark.cpp menu.cpp
HEADERS = AVLTree.hpp Set.hpp Person.hpp tests.hpp benchmark.hpp menu.hpp
OBJS = $(SRCS:.cpp=.o)
TARGET = lab1

.PHONY: all clean rebuild run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

rebuild: clean all