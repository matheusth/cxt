CFLAGS=-Wall -Wextra -std=c11 -pedantic -ggdb
LIBS=`pkg-config --libs sdl2` -lm -lSDL2_ttf

cxt: main.c buffer.c
	$(CC) $(CFLAGS) -o cxt main.c buffer.c $(LIBS)
