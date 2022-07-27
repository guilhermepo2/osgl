#include <SDL.h>
#include <stdio.h>

static const int WINDOW_WIDTH = 640;
static const int WINDOW_HEIGHT = 480;

int main() {
    SDL_Window* window = NULL;
    
    // Initializing SDL2
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL coult not be initialized!\n");
        printf("%s\n", SDL_GetError());
        return 1;
    }
    printf("SDL was initialized\n");

    // Creating the WINDOW!!
    window = SDL_CreateWindow(
        "osgl",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        0
    );

    if(window == NULL) {
        printf("error creating window!\n");
        printf("%s\n", SDL_GetError());
        return 1;
    }
    printf("windows was created!\n");

    int isRunning = 1;
    while(isRunning != 0) {
        SDL_Event e;
        while(SDL_PollEvent(&e) != 0) {
            switch(e.type) {
                case SDL_QUIT:
                    isRunning = 0;
                    break;
            }
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}