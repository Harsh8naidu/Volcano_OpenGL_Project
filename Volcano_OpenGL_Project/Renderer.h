// Renderer.h
#pragma once

#include <string>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include "Shader.h"

class Renderer {
public:
    Renderer();
    ~Renderer();

    bool Initialize();
    void Render();
    void Cleanup();

private:
    SDL_Window* window;
    SDL_GLContext context;

    Shader shader;
    GLuint VAO, VBO;
};
