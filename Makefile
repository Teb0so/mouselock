CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -g
PKG_CONFIG = pkg-config

CFLAGS += $(shell $(PKG_CONFIG) --cflags libevdev)
LDLIBS += $(shell $(PKG_CONFIG) --libs libevdev)

TARGET = mlock
SRC = mlock.c
OBJ = $(SRC:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
