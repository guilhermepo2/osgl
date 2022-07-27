#include <SDL.h>
#include <glad/glad.h>
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

    // setting opengl version 3.3
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

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

    // creating opengl context
    SDL_GLContext glContext;
    glContext = SDL_GL_CreateContext(window);

    if(glContext == NULL) {
        printf("error creating GL context!\n");
        printf("%s\n", SDL_GetError());
        return 1;
    }
    SDL_GL_MakeCurrent(window, glContext);
    printf("opengl context was created!\n");

    // Loading GLAD (time for the truth ?)
    if(!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
        printf("failed to load GLAD!\n");
        printf("%s\n", SDL_GetError());
    }
    printf("OpenGL Loaded\n");
    printf("Vendor: %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version: %s\n", glGetString(GL_VERSION));

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