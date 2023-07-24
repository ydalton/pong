##
# Project Title
#
# @file
# @version 0.1

CFLAGS :=-lSDL2 -lm -Wall -Wextra -Wpedantic -O2
CC := cc
OBJ := \
	main.o \
	game.o \
	draw.o

ifeq ($(shell uname -s), Linux)
	OBJ += beep.o
endif

test: $(OBJ)
	$(CC) -o test $(OBJ) $(CFLAGS)

clean:
	rm -f test $(OBJ)

.PHONY: test clean

# end
