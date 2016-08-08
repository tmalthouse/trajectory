TEST_OBJECTS=main.o test_vector3d.o
OBJECTS = $(TEST_OBJECTS) ../vector3d.o
OUTPUT=vector_test
LDFLAGS = -lm

.PHONY: clean

all: $(TEST_OBJECTS)
	gcc $(OBJECTS) -o$(OUTPUT)

clean:
	rm -f *.o $(OUTPUT)
