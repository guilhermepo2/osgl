COMPILER = gcc
INCLUDE = -I/opt/homebrew/include/SDL2 -D_THREAD_SAFE
LIBS = -L/opt/homebrew/lib
LIB_FLAGS = -lSDL2
OUTPUT = main

build:
	$(COMPILER) main.c $(INCLUDE) $(LIBS) $(LIB_FLAGS) -o $(OUTPUT)

run:
	./$(OUTPUT)

clean:
	rm $(OUTPUT)