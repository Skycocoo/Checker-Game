// Yuxi Luo (yl4217), February 26, 2018
// CS3113 Game Programming

#ifndef SETUP_H
#define SETUP_H

#include <SDL.h>
#include "stb_image.h"
#include "ShaderProgram.h"

// from lecture slide Jan 31, 2018
GLuint LoadTexture(const char *filePath);

// untextured shader
ShaderProgram setUntextured();

// textured shader
ShaderProgram setTextured(const std::string& filepath, GLuint& texture);

// initialize the window
SDL_Window* setUp(const std::string& name);

// check keyboard event
void checkKeyboard(const SDL_Event& event, bool& done);

#endif /* setUp_h */
