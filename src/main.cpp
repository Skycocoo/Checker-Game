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

enum GameMode {STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER};
GameMode mode;

using namespace std;

///////////////////////////////////////////GLOBAL VARIABLES///////////////////////////////////////////

float screenRatio = 0.0, screenHeight = 0.0, screenWidth = 0.0, splitScale = 0.0, edge = 1.0;
ShaderProgram textured;
SDL_Window* displayWindow;

float fixedStep = 0.0166666f; // 60 FPS (1.0f/60.0f) (update sixty times a second)
int maxStep = 3;

glm::vec3 center = glm::vec3(0, 0, 0);

//////////////////////////////////////////////////////////////////////////////////////////////////////

void updateGame(const SDL_Event& event, GameState& game){
    switch (event.type){
        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode){

                case SDL_SCANCODE_1:
                    if (mode == STATE_MAIN_MENU) game.setOrder(1);
                    break;
                case SDL_SCANCODE_2:
                    if (mode == STATE_MAIN_MENU) game.setOrder(2);
                    break;
                case SDL_SCANCODE_A:
                    if (mode == STATE_MAIN_MENU) game.setDiff(1);
                    break;
                case SDL_SCANCODE_B:
                    if (mode == STATE_MAIN_MENU) game.setDiff(2);
                    break;
                case SDL_SCANCODE_C:
                    if (mode == STATE_MAIN_MENU) game.setDiff(3);
                    break;
            }
            break;
    }
}



int main() {
    // initial set up
    displayWindow = setUp("Checker Game");
    mode = STATE_MAIN_MENU;

    GameState game;

    // Checker c;
    // c.play();

    SDL_Event event;
    bool done = false;

    uint button;
    int mouseX, mouseY;

    while (!done) {

        while (SDL_PollEvent(&event)) {
            checkKeyboard(event, done, button, mouseX, mouseY);
            updateGame(event, game);
        }

        if (mode == STATE_GAME_LEVEL) {
            game.mouse(button, mouseX, mouseY, done);
        } else {
            game.update();
            glClear(GL_COLOR_BUFFER_BIT);
            game.render();
            SDL_GL_SwapWindow(displayWindow);
        }

    }

    // c.determineWinner();
    std::cout << "------------End of Game------------" << std::endl;

    SDL_Quit();


}
