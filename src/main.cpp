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

using namespace std;

///////////////////////////////////////////GLOBAL VARIABLES///////////////////////////////////////////

float screenRatio = 0.0, screenHeight = 0.0, screenWidth = 0.0, splitScale = 0.0, edge = 1.0;
ShaderProgram textured;

float fixedStep = 0.0166666f; // 60 FPS (1.0f/60.0f) (update sixty times a second)
int maxStep = 3;

glm::vec3 center = glm::vec3(0, 0, 0);

//////////////////////////////////////////////////////////////////////////////////////////////////////


int main() {
    // initial set up
    SDL_Window* displayWindow = setUp("Checker Game");

    Checker c;
    // c.play();

    SDL_Event event;
    bool done = false;
    int mouseX, mouseY;
    while (!done) {
        while (SDL_PollEvent(&event)) {
            checkKeyboard(event, done);
        }
        uint botton = SDL_GetMouseState(&mouseX, &mouseY);
        if (botton == 1) cout << mouseX / 120 << " " << mouseY / 120 << endl;

        c.update();

        // display
        glClear(GL_COLOR_BUFFER_BIT);

        c.render();

        SDL_GL_SwapWindow(displayWindow);
    }
    SDL_Quit();


}
