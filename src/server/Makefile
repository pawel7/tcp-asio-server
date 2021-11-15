CXX=g++
ASIO_ROOT=../asio
CXXFLAGS=-std=c++2a -Wall -Wextra -fno-inline -I$(ASIO_ROOT)/include -g -DASIO_ENABLE_HANDLER_TRACKING -pthread
SOURCE=$(wildcard *.cpp)
TARGET=../../bin/
PROGRAMS=$(SOURCE:.cpp=)
#PROG_PATH = ../bin/
DSYM=$(SOURCE:.cpp=.dSYM)

all: $(PROGRAMS)

clean:
	rm -rf $(PROGRAMS) $(DSYM)
