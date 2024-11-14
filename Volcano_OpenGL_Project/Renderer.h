// Renderer.h
#pragma once

#include <string>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include "Shader.h"

#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

    // Transformation matrices
    glm::mat4 modelMat;
    glm::mat4 viewMat;
    glm::mat4 projMat;

    // Uniform locations
    GLint modelLoc;
    GLint viewLoc;
    GLint projLoc;

	void SetUpMatrices();
    
};
