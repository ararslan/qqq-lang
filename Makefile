CC=g++
SOURCES=./src/qqq.cpp
EXECUTABLE=./bin/qqq

all:
	$(CC) $(SOURCES) -o $(EXECUTABLE) -Wall -O0

clean:
	rm $(EXECUTABLE)
