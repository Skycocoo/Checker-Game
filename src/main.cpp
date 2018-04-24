// Created by Yuxi Luo, April 2018
// For CS4613 Artificial Intelligence; Checker Game project

#include <iostream>
#include <vector>
#include <limits>
#include <string>

#include "includes.h"

#define RESOURCE_FOLDER "../src/asset/"
#define STB_IMAGE_IMPLEMENTATION
#include "extern/stb_image.h"

///////////////////////////////////////////GLOBAL VARIABLES///////////////////////////////////////////

float screenRatio = 0.0, screenHeight = 0.0, screenWidth = 0.0, splitScale = 0.0, edge = 1.0;
ShaderProgram textured, untextured;

float fixedStep = 0.0166666f; // 60 FPS (1.0f/60.0f) (update sixty times a second)
int maxStep = 3;

glm::vec3 center = glm::vec3(0, 0, 0);

//////////////////////////////////////////////////////////////////////////////////////////////////////


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

using namespace std;

int main() {
    // initial set up
    // srand(time(NULL));
    SDL_Window* displayWindow = setUp("Checker Game");

    GLuint text;
    textured = setTextured("font.png", text);
    Text t(&textured, text);

    GLuint background;
    textured = setTextured("background.png", background);
    Object b(&textured, background);
    b.setShape(glm::vec3(screenWidth, screenHeight, 0));
    b.setScale(2*screenHeight);

    SDL_Event event;
    bool done = false;
    while (!done) {
        while (SDL_PollEvent(&event)) {
            checkKeyboard(event, done);
        }

        b.update();

        // display
        glClear(GL_COLOR_BUFFER_BIT);

        t.render("Platformer", 1, 2, 0, 3.5);
        b.render();

        SDL_GL_SwapWindow(displayWindow);
    }
    SDL_Quit();

    // Checker c;
    // c.play();
}












// from lecture slide Jan 31, 2018
GLuint LoadTexture(const char *filePath) {
    int w, h, comp;
    unsigned char* image = stbi_load(filePath, &w, &h, &comp, STBI_rgb_alpha);

    if(image == NULL) {
        cerr << "Unable to load image in the path " << *filePath << ". Make sure the path is correct\n";
        exit(1);
    }

    GLuint retTexture;
    glGenTextures(1, &retTexture);
    glBindTexture(GL_TEXTURE_2D, retTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(image);
    return retTexture;
}

// untextured shader
ShaderProgram setUntextured(){
    ShaderProgram program;
    program.Load(RESOURCE_FOLDER"shader/vertex.glsl", RESOURCE_FOLDER"shader/fragment.glsl");
    glUseProgram(program.programID);

    return program;
}

// textured shader
ShaderProgram setTextured(const string& filepath, GLuint& texture){
    ShaderProgram program;
    program.Load(RESOURCE_FOLDER"shader/vertex_textured.glsl", RESOURCE_FOLDER"shader/fragment_textured.glsl");
    texture = LoadTexture((RESOURCE_FOLDER + filepath).c_str());

    return program;
}


// initialize the window
SDL_Window* setUp(const string& name){
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* displayWindow = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1020, 720, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

    glViewport(0, 0, 1020, 720);
    glClearColor(0.376, 0.509, 0.806, 0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // screen view point
    screenRatio = float(1020) / float(720);
    screenHeight = 5.0;
    screenWidth = screenHeight * screenRatio;

    return displayWindow;
}

// check keyboard event
void checkKeyboard(const SDL_Event& event, bool& done){
    switch (event.type){
        case SDL_QUIT:
            done = true;
            break;
        case SDL_WINDOWEVENT_CLOSE:
            done = true;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.scancode == SDL_SCANCODE_Q) done = true;
            break;
    }
}
