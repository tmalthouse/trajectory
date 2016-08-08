CC = gcc
CFLAGS = -O3
LDFLAGS = -lSDL2 -lSDL2_gfx -lconfig -lm

OUTPUT = trajectory

SOURCES=body.c color.c debug.c main.c saveload.c vector2d.c vector3d.c
OBJECTS=$(SOURCES:.c=.o)

SDL_OBJECTS = SDL_coords.o SDL_main.o

SDL_OBJECTS = SDL/SDL_main.o SDL/SDL_coords.o

.PHONY: SDL clean v3d_test

all:  SDL $(OBJECTS)
	gcc $(OBJECTS) $(SDL_OBJECTS) $(LDFLAGS) -o$(OUTPUT)

SDL:
	cd SDL && make

v3d_test: vector3d.o
	cd Tests && make
	@echo "Running tests!"
	Tests/vector_test

clean:
	rm -f *.o $(OUTPUT)
	cd SDL && make clean
	cd Tests && make clean
