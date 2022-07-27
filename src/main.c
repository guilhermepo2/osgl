#include <SDL.h>
#include <glad/glad.h>
#include <stdio.h>

static const int WINDOW_WIDTH = 640;
static const int WINDOW_HEIGHT = 480;

const char* vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\n\0";

unsigned int shaderProgram;
unsigned int VBO, VAO, IBO;

float vertices[] = {
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
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

    // if code gets here, then everything is... good?
    // setting things up to render
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
    printf("created vertex shader\n");

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
    printf("created fragment shader\n");

    shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
    printf("linked vertex and fragment shader\n");

    glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

    glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

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
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
        SDL_GL_SwapWindow(window);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}