#pragma once
#include "SDL.h"
#include <iostream>

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Texture* texture;

void initialize(int windowWidth, int windowHeight, int textureWidth, int textureHeight);
void update(const void* buffer, int pitch);
void shutdown();