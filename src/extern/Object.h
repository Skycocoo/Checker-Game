// Yuxi Luo (yl4217), February 26, 2018
// CS3113 Game Programming

#ifndef OBJECT_H
#define OBJECT_H

#include "XMLLoad.h"
#include "ShaderProgram.h"
#include "glm/glm.hpp"
#include <vector>

class Object{
public:
    // constructor
    Object();
    Object(ShaderProgram* program, GLuint texture = 0, const glm::vec3& pos = glm::vec3(0, 0, 0));
    Object(ShaderProgram* program, GLuint texture, const XMLData& data, const glm::vec3& pos = glm::vec3(0, 0, 0));

    // update shader to render
    virtual void update(float elapsed = 0);
    // render the object
    virtual void render(const Matrix& view = Matrix());

    // set scale of object
    void setScale(float size);
    // set shape of object
    void setShape(const glm::vec3& shape);
    // set rotation of object
    void setRotate(float rot); // rotate: angle (in radian)
    // set position of object
    void setPos(const glm::vec3& pos);
    // set position of object
    void setPos(float x, float y);

    // set data from xml to load texture
    void setData(const XMLData& data);

protected:

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
    // std::vector<float> vertices = {-0.5, 0.5, 0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, -0.5, -0.5};
    std::vector<float> texCoords = {0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};

};

#endif
