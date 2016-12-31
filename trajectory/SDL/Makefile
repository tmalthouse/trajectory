SDL_OBJECTS = SDL_main.o SDL_coords.o

.PHONY: clean

SDL_main.o:
	$(CC) $(CFLAGS) -I/usr/local/include -c -o SDL_main.o SDL_main.c

SDL_coords.o:
	$(CC) $(CFLAGS) -I/usr/local/include -c -o SDL_coords.o SDL_coords.c

all: SDL_main.o SDL_coords.o
	echo $(CFLAGS)

clean:
	rm -f *.o
