// Shader.h
#pragma once

#include <string>
#include <GL/glew.h>
#include <glm/fwd.hpp>

class Shader {
public:
    Shader();
    ~Shader();

    bool Load(const std::string& vertexPath, const std::string& fragmentPath);
    void Use() const;
    GLuint GetProgram() const;
    void SetProjectionMatrix(const glm::mat4& projection);
    void SetViewMatrix(const glm::mat4& view);
    void SetModelMatrix(const glm::mat4& model);

private:
    GLuint program;

    std::string LoadShaderSource(const std::string& filePath);
    GLuint CompileShader(const char* source, GLenum type);
    bool LinkProgram(GLuint vertexShader, GLuint fragmentShader);
};
