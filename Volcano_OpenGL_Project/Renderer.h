#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Model_Loader.h"
#include "Shader.h"
#include <string>
#include "Camera.h"

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
    GLuint VAO, VBO, EBO;

    Model_Loader volcano_model;
    Shader shader;

    GLint modelLoc, viewLoc, projLoc;
    glm::mat4 modelMat, viewMat, projMat;

    std::vector<Vertex> vertices;  // Updated to use Vertex structure from ModelLoader
	std::vector<unsigned int> indices;
    size_t vertexCount;

    float angle;

    void SetUpMatrices();
    bool LoadModelData();
    bool LoadTexture(const std::string& filePath);

    GLuint textureID;
    GLuint textureLocation;

	Camera camera;
};

#endif // RENDERER_H
