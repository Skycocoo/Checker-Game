// Yuxi Luo (yl4217), March 1, 2018
// CS3113 Game Programming

#include "../extern/Text.h"
using namespace std;

extern float screenWidth;
extern float screenHeight;
extern float splitScale;

Text::Text(){}

Text::Text(ShaderProgram* program, GLuint texture): Object::Object(program, texture){
    projectionMatrix.SetOrthoProjection(-screenWidth, screenWidth, -screenHeight, screenHeight, -1.0f, 1.0f);
}

void Text::render(const string& text, float size, float spacing, float x, float y){
    program->SetModelMatrix(modelMatrix);
    program->SetProjectionMatrix(projectionMatrix);
    program->SetViewMatrix(viewMatrix);

    float shrink = 5.0;
    float texture_size = 1.0/16.0;

    // center the text
    x = x - float(text.size() - 1) / float(2) * (size + spacing) / shrink;

    vector<float> vertexData;
    vector<float> texCoordData;
    for (int i = 0; i < text.size(); i++) {
        int spriteIndex = (int)text[i];
        float texture_x = (float)(spriteIndex % 16) / 16.0f;
        float texture_y = (float)(spriteIndex / 16) / 16.0f;
        vertexData.insert(vertexData.end(), {
            ((size+spacing) * i)/shrink + (-0.5f * size) + x, 0.5f * size + y,
            ((size+spacing) * i)/shrink + (-0.5f * size) + x, -0.5f * size + y,
            ((size+spacing) * i)/shrink + (0.5f * size) + x, 0.5f * size + y,
            ((size+spacing) * i)/shrink + (0.5f * size) + x, -0.5f * size + y,
            ((size+spacing) * i)/shrink + (0.5f * size) + x, 0.5f * size + y,
            ((size+spacing) * i)/shrink + (-0.5f * size) + x, -0.5f * size + y,
        });
        texCoordData.insert(texCoordData.end(), {
            texture_x, texture_y,
            texture_x, texture_y + texture_size,
            texture_x + texture_size, texture_y,
            texture_x + texture_size, texture_y + texture_size,
            texture_x + texture_size, texture_y,
            texture_x, texture_y + texture_size,
        }); }

    glUseProgram(program->programID);
    glBindTexture(GL_TEXTURE_2D, texture);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
    glEnableVertexAttribArray(program->positionAttribute);
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
    glEnableVertexAttribArray(program->texCoordAttribute);

    glDrawArrays(GL_TRIANGLES, 0, text.size() * 6);
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}
