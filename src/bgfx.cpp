#include <SDL.h>
#include <iostream>

static const int WINDOW_WIDTH = 640;
static const int WINDOW_HEIGHT = 480;
static SDL_Window* window = nullptr;

int main(void) {
    
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Error initializing SDL2" << std::endl;
        return 1;
    }

    window = SDL_CreateWindow("bgfx", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (window == nullptr) {
        std::cout << "Error creating SDL window" << std::endl;
        return 1;
    }

    bool isRunning = true;

    while(isRunning) {
        SDL_Event e;
        while(SDL_PollEvent(&e) != 0) {
            switch(e.type) {
                case SDL_QUIT:
                isRunning = false;
                break;
            }
        }

        // SDL_GL_SwapWindow(window);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}