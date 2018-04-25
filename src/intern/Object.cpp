// Yuxi Luo (yl4217), February 26, 2018
// CS3113 Game Programming

#include "Object.h"
#include <math.h>

using namespace std;

extern float screenWidth;
extern float screenHeight;
extern float splitScale;

// constructor
Object::Object(){}

Object::Object(ShaderProgram* program, GLuint texture, const glm::vec3& pos):
program(program), texture(texture), pos(pos), shape(1, 1, 1){
    // set projection matrix to corresponding projection on 2D
    projectionMatrix.SetOrthoProjection(-screenWidth, screenWidth, -screenHeight, screenHeight, -1.0f, 1.0f);
}

Object::Object(ShaderProgram* program, GLuint texture, const XMLData& data, const glm::vec3& pos):
program(program), texture(texture), pos(pos){
    // set projection matrix to corresponding projection on 2D
    projectionMatrix.SetOrthoProjection(-screenWidth, screenWidth, -screenHeight, screenHeight, -1.0f, 1.0f);
    // set data from xml to load texture
    setData(data);
}


// update shader to render
void Object::update(float elapsed){
    // need to reset model matrix to update the position
    modelMatrix.Identity();

    modelMatrix.Translate(pos.x, pos.y, pos.z);
    modelMatrix.Scale(scale, scale, scale);

    modelMatrix.Rotate(rotate);
}


// render the object
void Object::render(const Matrix& view){
    // set the shader program to corresponding matrices
    program->SetModelMatrix(modelMatrix);
    program->SetProjectionMatrix(projectionMatrix);
    program->SetViewMatrix(view);

    // send vertices and attributes to shader
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

// set scale of object
void Object::setScale(float size){
    this->scale = size;
    this->shape *= size;
}

// set shape of object
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

// set rotation of object
void Object::setRotate(float rot){
    this->rotate = rot;
}

// set position of object
void Object::setPos(const glm::vec3& pos){
    this->pos = pos;
}

// set position of object
void Object::setPos(float x, float y){
    this->pos.x = x;
    this->pos.y = y;
}


// set data from xml to load texture
void Object::setData(const XMLData& data){
    // assume the shape of sheetsprite is 256 * 256
    float u = data.x / 256.0,
          v = data.y / 256.0,
          width = data.width / 256.0,
          height = data.height / 256.0;

    // rescale the image so that the max edge length is 1
    float w = (data.width / data.height < 1) ? data.width / data.height : 1.0,
          h = (data.width / data.height < 1) ? 1.0 : data.height / data.width;

    // redefine vertices and texture coordinates for the object
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
