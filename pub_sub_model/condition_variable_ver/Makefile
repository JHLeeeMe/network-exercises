CC=g++                        # (built-in variable) Compiler
CFLAGS=-g -Wall -pthread      # (built-in variable) extra flags to git to CC
OBJS=main.o
TARGET=main

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o bin/$@ $(OBJS) $(CFLAGS)   # $@: (automatic variables) current target name

main.o: main.cpp

clean:
	rm -f *.o net/*.o
	rm -f bin/$(TARGET)
