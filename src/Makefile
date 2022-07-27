COMPILER = gcc
INCLUDE = -I/opt/homebrew/include/SDL2 -D_THREAD_SAFE -I./glad/include
LIBS = -L/opt/homebrew/lib
LIB_FLAGS = -lSDL2
OUTPUT = main

build:
	$(COMPILER) main.c glad/src/glad.c $(INCLUDE) $(LIBS) $(LIB_FLAGS) -o $(OUTPUT)

run:
	./$(OUTPUT)

clean:
	rm $(OUTPUT)