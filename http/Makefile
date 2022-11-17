CC=g++                    # (built-in variable) Compiler
CFLAGS=-g -Wall -pthread           # (built-in variable) extra flags to git to CC
OBJS=main.o net/http_server.o net/http_client.o
TARGET=main

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o bin/$@ $(OBJS) $(CFLAGS)   # $@: (automatic variables) current target name

main.o: main.cpp net/http_interface.h net/http_server.h net/http_client.h
http_server.o: net/http_server.h net/http_server.cpp
http_client.o: net/http_client.h net/http_client.cpp

clean:
	rm -f *.o net/*.o
	rm -f bin/$(TARGET)
