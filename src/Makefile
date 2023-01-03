COMPILER = gcc
INCLUDE = -I/opt/homebrew/include/SDL2 -D_THREAD_SAFE -I./glad/include
LIBS = -L/opt/homebrew/lib
LIB_FLAGS = -lSDL2
OUTPUT = main

build:
	$(COMPILER) main.c glad/src/glad.c $(INCLUDE) $(LIBS) $(LIB_FLAGS) -o $(OUTPUT)

build-bgfx:
	g++ bgfx.cpp $(INCLUDE) $(LIBS) $(LIB_FLAGS) -o bgfx

build-sdl2r:
	g++ sdl2renderer.cpp $(INCLUDE) $(LIBS) $(LIB_FLAGS) -o sdl2r

run:
	./$(OUTPUT)

run-bgfx:
	./bgfx

run-sdl2r:
	./sdl2r

clean:
	rm $(OUTPUT)