#include <SDL.h>
#include <glad/glad.h>
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static const int WINDOW_WIDTH = 640;
static const int WINDOW_HEIGHT = 480;

unsigned char* LoadImage(const char* path, int* outImageWidth, int* outImageHeight, int* outComponentsPerPixel, int stride, int flip) {
    stbi_set_flip_vertically_on_load(flip);
    unsigned char* data = stbi_load(path, outImageWidth, outImageHeight, outComponentsPerPixel, stride);

    if(data == NULL) {
        printf("error loading image!\n");
    }

    return data;
}

const char* vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec2 aTexCoords;\n"
    "out vec2 TexCoord;\n"
	"void main()\n"
	"{\n"
    "   TexCoord = aTexCoords;\n"
	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
	"layout(location = 0) out vec4 FragColor;\n"
    "in vec2 TexCoord;\n"
    "uniform sampler2D ourTexture;\n"
	"void main()\n"
	"{\n"
	"   FragColor = texture(ourTexture, TexCoord);\n"
	"}\n\0";

unsigned int shaderProgram;
unsigned int VBO, VAO, IBO;

float vertices[] = {
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,  // top right
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f  // top left 
};

unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
};

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
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);

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

    glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // if code gets here, then everything is... good?
    // setting things up to render
    GLenum shaderError;
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
    printf("created vertex shader\n");

    shaderError = glGetError();
    if(shaderError == GL_NO_ERROR) {
        printf("no shader error!\n");
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
    printf("created fragment shader\n");
    shaderError = glGetError();
    if(shaderError == GL_NO_ERROR) {
        printf("no shader error!\n");
    }

    shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
    printf("linked vertex and fragment shader\n");
    shaderError = glGetError();
    if(shaderError == GL_NO_ERROR) {
        printf("no shader error!\n");
    }

    glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
    shaderError = glGetError();
    if(shaderError == GL_NO_ERROR) {
        printf("no shader error!\n");
    }

    // creating a texture!!
    int imageWidth;
    int imageHeight;
    int comppp;
    unsigned char* image = LoadImage("test.png", &imageWidth, &imageHeight, &comppp, 0, 1);
    unsigned int textureID;

    printf("creating texture...\n");
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    GLenum texError = glGetError();
    if(texError == GL_NO_ERROR) {
        printf("no texture error!\n");
    } else {
        switch(texError) {
            case GL_INVALID_ENUM:
            printf("GL_INVALID_ENUM %d\n", texError);
            break;
            case GL_INVALID_VALUE:
            printf("GL_INVALID_VALUE %d\n", texError);
            break;
            case GL_INVALID_OPERATION:
            printf("GL_INVALID_OPERATION %d\n", texError);
            break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
            printf("GL_INVALID_FRAMEBUFFER_OPERATION %d\n", texError);
            break;
            case GL_OUT_OF_MEMORY:
            printf("GL_OUT_OF_MEMORY %d\n", texError);
            break;
            case GL_STACK_UNDERFLOW:
            printf("GL_STACK_UNDERFLOW %d\n", texError);
            break;
            case GL_STACK_OVERFLOW:
            printf("GL_STACK_OVERFLOW %d\n", texError);
            break;
        }
        printf("error creating texture!! %d\n", texError);
    }

    
    stbi_image_free(image);

    printf("starting to run...\n");
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

        // Rendering Stuff!
        glClearColor(0.33f, 0.0f, 0.33f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(shaderProgram);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glUniform1i(glGetUniformLocation(shaderProgram, "ourTexure"), 0);
        
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}