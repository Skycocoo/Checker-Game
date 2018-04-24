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
    Object();
    Object(ShaderProgram* program, GLuint texture = 0, const glm::vec3& pos = glm::vec3(0, 0, 0));
    Object(ShaderProgram* program, GLuint texture, const XMLData& data, const glm::vec3& pos = glm::vec3(0, 0, 0));

    virtual void update(float elapsed = 0);
    virtual void render(const Matrix& view = Matrix());

    void setScale(float size);
    void setShape(const glm::vec3& shape);
    void setRotate(float rot); // rotate: angle (in radian)
    void setPos(const glm::vec3& pos);
    void setPos(float x, float y);

    void setProject(float proj);

    void setData(const XMLData& data);

    const glm::vec3& getPos() const;

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
