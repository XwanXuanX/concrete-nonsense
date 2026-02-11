CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -Werror -Wpedantic -Wconversion -Wshadow \
		   -Wcast-align -Wunused -Wno-unused-parameter -Wold-style-cast \
		   -Wpointer-arith -Wcast-qual -Wno-missing-braces
SANITIZERS = -fsanitize=undefined -fsanitize=thread
DEBUG_FLAGS = -O0
THREAD_FLAGS = -pthread

SOURCES = $(wildcard *.cc)
TARGETS = $(SOURCES:.cc=.o)

.PHONY: all clean

all: $(TARGETS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) $(DEBUG_FLAGS) $(THREAD_FLAGS) $(SANITIZERS) -o $@ $<

clean:
	rm -f $(TARGETS)
	rm -rf *.dSYM
