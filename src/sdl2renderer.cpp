#include <SDL.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static const int WINDOW_WIDTH = 640;
static const int WINDOW_HEIGHT = 480;
static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;

unsigned char* LoadImage(const char* path, int* outImageWidth, int* outImageHeight, int* outComponentsPerPixel, int stride, int flip) {
    stbi_set_flip_vertically_on_load(flip);
    unsigned char* data = stbi_load(path, outImageWidth, outImageHeight, outComponentsPerPixel, stride);

    if(data == NULL) {
        printf("error loading image!\n");
    }

    return data;
}

int main(void) {
    
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cout << "Error initializing SDL2" << std::endl;
        return 1;
    }

    window = SDL_CreateWindow("bgfx", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (window == nullptr) {
        std::cout << "Error creating SDL window" << std::endl;
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == nullptr) {
        std::cout << "error creating SDL renderer" << std::endl;
        return 1;
    }

    bool isRunning = true;

    int imageWidth;
    int imageHeight;
    int bpp;
    unsigned char* image = LoadImage("test.png", &imageWidth, &imageHeight, &bpp, 0, 0);
    int pitch = imageWidth * bpp;
    pitch = (pitch + 3) & ~3;

    int32_t rmask, gmask, bmask, amask;
    rmask = 0x000000FF;
    gmask = 0x0000FF00;
    bmask = 0x00FF0000;
    amask = (bpp == 4) ? 0xFF000000 : 0;

    SDL_Surface* imgSurface = SDL_CreateRGBSurfaceFrom(
        image, 
        imageWidth, 
        imageHeight, 
        bpp * 8, 
        pitch,
        rmask,
        gmask,
        bmask,
        amask
    );

    if(imgSurface == nullptr) {
        std::cout << "error creating surface!" << std::endl;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, imgSurface);
    SDL_FreeSurface(imgSurface);

    while(isRunning) {
        SDL_Event e;
        while(SDL_PollEvent(&e) != 0) {
            switch(e.type) {
                case SDL_QUIT:
                isRunning = false;
                break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 100, 21, 21, 255);
	    SDL_RenderClear(renderer);

        SDL_Rect dest = {
            10,
            10,
            imageWidth,
            imageHeight
        };
        
        SDL_RenderCopyEx(
            renderer,
            texture,
            nullptr,
            &dest,
            0,
            nullptr,
            SDL_FLIP_NONE
        );

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}