#include <iostream>
#include "Renderer.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Renderer::Renderer() : window(nullptr), context(nullptr), VAO(0), VBO(0) {}

Renderer::~Renderer() {
    Cleanup();
}

bool Renderer::Initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    window = SDL_CreateWindow("Volcano OpenGL Project", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    if (!window) {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    context = SDL_GL_CreateContext(window);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    if (!context) {
        SDL_DestroyWindow(window);
        std::cout << "OpenGL context could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    if (glewInit() != GLEW_OK) {
        std::cout << "GLEW could not initialize!" << std::endl;
        return false;
    }

    // Load shaders
    if (!shader.Load("vertex_shader.glsl", "fragment_shader.glsl")) {
        std::cerr << "Failed to load shaders!" << std::endl;
        return false;
    }

    // Get uniform locations from the shader
    modelLoc = glGetUniformLocation(shader.GetProgram(), "modelMat");
    viewLoc = glGetUniformLocation(shader.GetProgram(), "viewMat");
    projLoc = glGetUniformLocation(shader.GetProgram(), "projMat");

    SetUpMatrices();

    // Load texture
    /*if (!LoadTexture("volcano_texture.png")) {
        std::cerr << "Failed to load texture!" << std::endl;
        return false;
    }
    else {
        std::cout << "Texture loaded successfully!" << std::endl;
    }*/

    // Load and bind model data
    if (!LoadModelData()) {
        std::cerr << "Failed to load model data!" << std::endl;
        return false;
    }
    else {
        std::cout << "Model data loaded successfully!" << std::endl;
    }

    glEnable(GL_DEPTH_TEST);
    return true;
}

bool Renderer::LoadModelData() {
    if (!volcano_model.LoadOBJ("volcano_model.obj")) {
        std::cerr << "Failed to load OBJ file!" << std::endl;
        return false;
    }

    vertices = volcano_model.vertices;
    indices = volcano_model.indices; // Ensure indices are stored here
    vertexCount = indices.size();

    if (vertices.empty() || indices.empty()) {
        std::cerr << "Failed to load vertex data from model!" << std::endl;
        return false;
    }

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &EBO); // Element Buffer for indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0); // Unbind VAO
    return true;
}

bool Renderer::LoadTexture(const std::string& filePath) {
    int width, height, channels;
    unsigned char* image = stbi_load(filePath.c_str(), &width, &height, &channels, 0);

    if (!image) {
        std::cerr << "Failed to load texture: " << filePath << std::endl;
        return false;
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Load texture into OpenGL
    if (channels == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    else if (channels == 4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(image); // Free the image data after loading

    return true;
}


void Renderer::SetUpMatrices() {
    float ar = 800.0f / 600.0f; // Aspect ratio
    modelMat = glm::mat4(1.0f); // Identity matrix for model
    //viewMat = glm::lookAt(glm::vec3(40.0f, 20.0f, -55.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.f, 1.f, 0.f));
    projMat = glm::perspective(glm::radians(50.0f), ar, 0.1f, 5000.0f); // Perspective projection

    camera.MoveTo(glm::vec3(40.0f, 20.0f, -55.0f));
	camera.TurnTo(glm::vec3(0.f, 0.f, 0.f));

    // Send matrices to the shader
    shader.Use();
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &modelMat[0][0]);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &viewMat[0][0]);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projMat[0][0]);
}

void Renderer::Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader.Use();

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &modelMat[0][0]);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0); // Use glDrawElements with indices
    SDL_GL_SwapWindow(window);
}

void Renderer::Cleanup() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
