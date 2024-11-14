#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include <vector>
#include <string>
#include <glm/glm.hpp> // GLM library for vector types

// Structure to hold vertex data
struct Vertex {
    glm::vec3 position;
    glm::vec2 texCoord;
    glm::vec3 normal;
};

// ModelLoader class to handle loading the OBJ file
class Model_Loader {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    Model_Loader() = default;

    // Constructor
    Model_Loader(const std::string& filePath);

    // Loads the OBJ file from the given file path
    bool LoadOBJ(const std::string& filePath);
};

#endif // MODEL_LOADER_H
