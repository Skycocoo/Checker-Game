// Yuxi Luo (yl4217), March 1, 2018
// CS3113 Game Programming

#ifndef Text_h
#define Text_h

#include "ShaderProgram.h"
#include "glm/glm.hpp"
#include <vector>

class Text{
public:
    Text();
    Text(ShaderProgram* program, GLuint texture);
    void render(const std::string& text, float size, float spacing, float x, float y);

private:
    // shader
    ShaderProgram* program;

    // texture
    GLuint texture;

    // matrices
    Matrix projectionMatrix;
    Matrix modelMatrix;
    Matrix viewMatrix;

    // physics
    glm::vec3 pos;
    float rotate = 0.0;
    float scale = 1.0;

    // vertices
    glm::vec3 shape; // shape.x: width; shape.y: height; shape.z: 0 / 1
    std::vector<float> vertices = {-0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5};
    std::vector<float> texCoords = {0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};

};

#endif /* Text_h */
