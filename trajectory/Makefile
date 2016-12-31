CC = gcc
CFLAGS = -static -O3 -I/usr/local/include
LDFLAGS = -lSDL2_gfx -lsqlite3  -L/usr/local/lib -lSDL2 -lm -liconv -Wl,-framework,CoreAudio -Wl,-framework,AudioToolbox -Wl,-framework,ForceFeedback -lobjc -Wl,-framework,CoreVideo -Wl,-framework,Cocoa -Wl,-framework,Carbon -Wl,-framework,IOKit

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
