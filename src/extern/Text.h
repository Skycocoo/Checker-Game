// Yuxi Luo (yl4217), March 1, 2018
// CS3113 Game Programming

#ifndef Text_h
#define Text_h

#include "Object.h"

class Text: public Object{
public:
    Text();
    Text(ShaderProgram* program, GLuint texture);
    void render(const std::string& text, float size, float spacing, float x, float y);
    void renderLeft(const std::string& text, float size, float spacing, float x, float y);
private:

};

#endif /* Text_h */
