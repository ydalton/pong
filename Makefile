CFLAGS :=-lSDL2 -lm -Wall -Wextra -Wpedantic -O2
CC := cc
OBJ := \
	main.o \
	game.o \
	draw.o

ifeq ($(shell uname -s), Linux)
	OBJ += beep.o
endif

pong: $(OBJ)
	$(CC) -o pong $(OBJ) $(CFLAGS)

clean:
	rm -f test $(OBJ)

.PHONY: test clean
