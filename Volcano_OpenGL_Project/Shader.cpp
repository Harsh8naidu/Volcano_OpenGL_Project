// Shader.cpp
#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>

#include "Renderer.h"

Shader::Shader() : program(0) {}

Shader::~Shader() {
    if (program != 0) {
        glDeleteProgram(program);
    }
}

bool Shader::Load(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vertexSource = LoadShaderSource(vertexPath);
    std::string fragmentSource = LoadShaderSource(fragmentPath);

    if (vertexSource.empty() || fragmentSource.empty()) {
        return false;
    }

    GLuint vertexShader = CompileShader(vertexSource.c_str(), GL_VERTEX_SHADER);
    GLuint fragmentShader = CompileShader(fragmentSource.c_str(), GL_FRAGMENT_SHADER);

    if (!LinkProgram(vertexShader, fragmentShader)) {
        return false;
    }

    // Clean up individual shaders after linking
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return true;
}

void Shader::SetProjectionMatrix(const glm::mat4& projMat) {
	glUniformMatrix4fv(glGetUniformLocation(program, "projMat"), 1, GL_FALSE, glm::value_ptr(projMat));
}

void Shader::SetViewMatrix(const glm::mat4& viewMat) {
	glUniformMatrix4fv(glGetUniformLocation(program, "viewMat"), 1, GL_FALSE, glm::value_ptr(viewMat));
}

void Shader::SetModelMatrix(const glm::mat4& modelMat) {
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMat"), 1, GL_FALSE, glm::value_ptr(modelMat));
}

void Shader::Use() const {
    glUseProgram(program);
}

GLuint Shader::GetProgram() const {
    return program;
}

std::string Shader::LoadShaderSource(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << filePath << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

GLuint Shader::CompileShader(const char* source, GLenum type) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return shader;
}

bool Shader::LinkProgram(GLuint vertexShader, GLuint fragmentShader) {
    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        return false;
    }

    return true;
}