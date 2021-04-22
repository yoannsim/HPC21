CC=gcc

CFLAGS=-g -O2 -Wall
LDFLAGS=

OBJS=$(pathsubst)

TARGET=ex2

.PHONY: clean all default

default: all

all: $(TARGET)

$(TARGET) : ex2.o
	$(CC) $(LDFLAGS) $^ -o $@

%.o : %.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -f $(TARGET)
	rm -f *.o
