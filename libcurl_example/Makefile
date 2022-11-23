CC=g++
CFLAGS=-g -Wall
INCLUDE_PATH=/home/jhleeeme/build/include/
LIB_PATH=/home/jhleeeme/build/lib/
OBJS=main.o
ODIR=./bin
TARGET=main

dummy_build_folder := $(shell mkdir -p $(ODIR))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(ODIR)/$@ $(OBJS) $(CFLAGS) -L$(LIB_PATH) -lcurl

$(OBJS): main.cpp
	$(CC) -c -o $(OBJS) main.cpp -I$(INCLUDE_PATH)

clean:
	rm -f *.o
	rm -f bin/$(TARGET)
