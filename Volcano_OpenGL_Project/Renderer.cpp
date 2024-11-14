// Renderer.cpp
#include "Renderer.h"
#include <iostream>
#include <fstream>
#include <sstream>

Renderer::Renderer() : window(nullptr), context(nullptr), VAO(0), VBO(0) {}

Renderer::~Renderer() {
    Cleanup();
}

bool Renderer::Initialize() {
    // SDL and OpenGL setup
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    window = SDL_CreateWindow("Volcano OpenGL Project", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);

    if (!window) {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    context = SDL_GL_CreateContext(window);

    if (!context) {
        SDL_DestroyWindow(window);
        std::cout << "OpenGL context could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    if (glewInit() != GLEW_OK) {
        std::cout << "GLEW could not initialize!" << std::endl;
        return false;
    }

    // Initialize shaders
    if (!shader.Load("vertex_shader.glsl", "fragment_shader.glsl")) {
        std::cerr << "Failed to load shaders!" << std::endl;
        return false;
    }

    // Vertex setup
    float vertices[] = {
        -1.f, -1.f, 0.f,
         1.f, -1.f, 0.f,
         0.f,  1.f, 0.f
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    return true;
}

void Renderer::Render() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

	shader.Use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    SDL_GL_SwapWindow(window);
}

void Renderer::Cleanup() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}