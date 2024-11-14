// Volcano_OpenGL_Project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <Windows.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/GL.h>
#include "Shader.h"

#undef main

SDL_Window* window;
SDL_GLContext context;

float vertices[] = {
	-1.f, -1.f, 0.f,
	 1.f, -1.f, 0.f,
	 0.f,  1.f, 0.f
};

int main()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	window = SDL_CreateWindow("Volcano OpenGL Project", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);

	if (window == NULL)
	{
		std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	context = SDL_GL_CreateContext(window);

	if (context == NULL)
	{
		SDL_DestroyWindow(window);
		SDL_Quit();
		std::cout << "OpenGL context could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	
	if (glewInit() != GLEW_OK)
	{
		SDL_GL_DeleteContext(context);
		SDL_DestroyWindow(window);
		SDL_Quit();
		std::cout << "GLEW could not initialize!" << std::endl;
		return 1;
	}

	unsigned int vs;
	unsigned int fs;
	unsigned int shader;
	
	// Load shader sources
	std::string vertexShaderSource = LoadShaderSource("vertex_shader.glsl");
	std::string fragmentShaderSource = LoadShaderSource("fragment_shader.glsl");

	// Convert to C-style string for OpenGL
	const char* vsSourceCStr = vertexShaderSource.c_str();
	const char* fsSourceCStr = fragmentShaderSource.c_str();

	// Create and compile the vertex shader
	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vsSourceCStr, NULL);
	glCompileShader(vs);

	// Check for compilation errors (optional)
	int success;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(vs, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Create and compile the fragment shader
	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fsSourceCStr, NULL);
	glCompileShader(fs);

	// Check for compilation errors (optional)
	glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(fs, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Link shaders into a shader program
	shader = glCreateProgram();
	glAttachShader(shader, vs);
	glAttachShader(shader, fs);
	glLinkProgram(shader);

	// Check for linking errors (optional)
	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// Clean up shaders (they are linked into the program now and can be deleted)
	glDeleteShader(vs);
	glDeleteShader(fs);
	
	unsigned int VAO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	bool stopped = false;

	while (!stopped)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				stopped = true;
			}
		}

		// render the scene
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		SDL_GL_SwapWindow(window);
	}

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
