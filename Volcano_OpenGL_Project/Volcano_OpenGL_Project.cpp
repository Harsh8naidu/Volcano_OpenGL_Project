// Volcano_OpenGL_Project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <Windows.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/GL.h>
#include "Renderer.h"

#undef main

int main() {
    Renderer renderer;

    if (!renderer.Initialize()) {
        std::cout << "Renderer failed to initialize!" << std::endl;
        return 1;
    }

    bool stopped = false;
    while (!stopped) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                stopped = true;
            }
        }

        renderer.Render();
    }

    renderer.Cleanup();
    return 0;
}
