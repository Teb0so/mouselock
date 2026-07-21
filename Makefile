CC = gcc

CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -g
CFLAGS += $(shell pkg-config --cflags libevdev)

LDLIBS = $(shell pkg-config --libs libevdev)

TARGET = mlock

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
