#pragma once

#include <SDL_opengl.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Matrix.h"

class ShaderProgram {
public:
	void Load(const char *vertexShaderFile, const char *fragmentShaderFile);
	void Cleanup();

    void SetModelMatrix(const Matrix &matrix);
    void SetProjectionMatrix(const Matrix &matrix);
    void SetViewMatrix(const Matrix &matrix);

    void SetColor(float r, float g, float b, float a);

    GLuint LoadShaderFromString(const std::string &shaderContents, GLenum type);
    GLuint LoadShaderFromFile(const std::string &shaderFile, GLenum type);

    GLuint programID;

    GLuint projectionMatrixUniform;
    GLuint modelMatrixUniform;
    GLuint viewMatrixUniform;
    GLuint colorUniform;

    GLuint positionAttribute;
    GLuint texCoordAttribute;

    GLuint vertexShader;
    GLuint fragmentShader;
};
