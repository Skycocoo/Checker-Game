// Yuxi Luo (yl4217), February 26, 2018
// CS3113 Game Programming

#include "Object.h"
#include <math.h>

using namespace std;

extern float screenWidth;
extern float screenHeight;
extern float splitScale;

Object::Object(){}

Object::Object(ShaderProgram* program, GLuint texture, const glm::vec3& pos):
program(program), texture(texture), pos(pos), shape(1, 1, 1){
    projectionMatrix.SetOrthoProjection(-screenWidth, screenWidth, -screenHeight, screenHeight, -1.0f, 1.0f);
}

Object::Object(ShaderProgram* program, GLuint texture, const XMLData& data, const glm::vec3& pos):
program(program), texture(texture), pos(pos){
    projectionMatrix.SetOrthoProjection(-screenWidth, screenWidth, -screenHeight, screenHeight, -1.0f, 1.0f);
    setData(data);
}


void Object::setProject(float proj){
    projectionMatrix.SetOrthoProjection(-screenWidth * proj, screenWidth * proj, -screenHeight * proj, screenHeight * proj, -1.0f, 1.0f);
}


void Object::update(float elapsed){
    modelMatrix.Identity();

    modelMatrix.Translate(pos.x, pos.y, pos.z);
    modelMatrix.Scale(scale, scale, scale);

    modelMatrix.Rotate(rotate);
}


void Object::render(const Matrix& view){
    // viewMatrix = view;

    program->SetModelMatrix(modelMatrix);
    program->SetProjectionMatrix(projectionMatrix);
    program->SetViewMatrix(view);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
    glEnableVertexAttribArray(program->positionAttribute);

    if (glIsTexture(texture)){
        glBindTexture(GL_TEXTURE_2D, texture);
        glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data());
        glEnableVertexAttribArray(program->texCoordAttribute);
    }
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program->positionAttribute);

    if (glIsTexture(texture)) glDisableVertexAttribArray(program->texCoordAttribute);
}

void Object::setScale(float size){
    this->scale = size;
    this->shape *= size;
}

void Object::setShape(const glm::vec3& shape){
    this->scale = 1.0;

    float w = shape.x / shape.y;
    float h = 1.0;
    vertices = {
        -0.5f * w, -0.5f * h,
        0.5f * w, -0.5f * h,
        0.5f * w, 0.5f * h,
        -0.5f * w, -0.5f * h,
        0.5f * w, 0.5f * h,
        -0.5f * w, 0.5f * h ,
    };
    this->shape.x = w;
    this->shape.y = h;
}

void Object::setRotate(float rot){
    this->rotate = rot;
}

void Object::setPos(const glm::vec3& pos){
    this->pos = pos;
}

void Object::setPos(float x, float y){
    this->pos.x = x;
    this->pos.y = y;
}

const glm::vec3& Object::getPos() const {
    return pos;
}

void Object::setData(const XMLData& data){
    // assume the shape of sheetsprite is 256 * 256
    float u = data.x / 256.0,
          v = data.y / 256.0,
          width = data.width / 256.0,
          height = data.height / 256.0;

    // rescale the image so that the max edge length is 1
    float w = (data.width / data.height < 1) ? data.width / data.height : 1.0,
          h = (data.width / data.height < 1) ? 1.0 : data.height / data.width;

    vertices = {
        -0.5f * w, -0.5f * h,
        0.5f * w, -0.5f * h,
        0.5f * w, 0.5f * h,
        -0.5f * w, -0.5f * h,
        0.5f * w, 0.5f * h,
        -0.5f * w, 0.5f * h ,
    };

    texCoords = {
        u, v+height,
        u+width, v+height,
        u+width, v,
        u, v+height,
        u+width, v,
        u, v
    };

    this->shape.x = w;
    this->shape.y = h;
}
