// Shader.h
#pragma once

#include <string>
#include <GL/glew.h>

class Shader {
public:
    Shader();
    ~Shader();

    bool Load(const std::string& vertexPath, const std::string& fragmentPath);
    void Use() const;
    GLuint GetProgram() const;

private:
    GLuint program;

    std::string LoadShaderSource(const std::string& filePath);
    GLuint CompileShader(const char* source, GLenum type);
    bool LinkProgram(GLuint vertexShader, GLuint fragmentShader);
};
