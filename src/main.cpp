// Created by Yuxi Luo, April 2018
// For CS4613 Artificial Intelligence; Checker Game project

#include <iostream>
#include <vector>
#include <limits>
#include <string>

#include "includes.h"

#define STB_IMAGE_IMPLEMENTATION
#include "extern/stb_image.h"

enum GameMode {STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER};
GameMode mode;

using namespace std;

///////////////////////////////////////////GLOBAL VARIABLES///////////////////////////////////////////

float screenRatio = 0.0, screenHeight = 0.0, screenWidth = 0.0;
ShaderProgram textured;
SDL_Window* displayWindow;

//////////////////////////////////////////////////////////////////////////////////////////////////////

void updateGame(const SDL_Event& event, GameState& game){
    switch (event.type){
        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode){
                case SDL_SCANCODE_1:
                    // player move first
                    if (mode == STATE_MAIN_MENU) game.setOrder(1);
                    break;
                case SDL_SCANCODE_2:
                    // player move second
                    if (mode == STATE_MAIN_MENU) game.setOrder(2);
                    break;
                case SDL_SCANCODE_A:
                    // set the level of difficulty to be easy
                    if (mode == STATE_MAIN_MENU) game.setDiff(1);
                    break;
                case SDL_SCANCODE_B:
                    // set the level of difficulty to be medium
                    if (mode == STATE_MAIN_MENU) game.setDiff(2);
                    break;
                case SDL_SCANCODE_C:
                    // set the level of difficulty to be hard
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

    SDL_Event event;

    uint button;
    int mouseX, mouseY;
    bool done = false;
    while (!done) {

        while (SDL_PollEvent(&event)) {
            // check inputs for each loop
            checkInput(event, done, button, mouseX, mouseY);
            // update game state if necessary
            updateGame(event, game);
        }

        if (mode == STATE_GAME_LEVEL) {
            // if in the game level: process player's selection & update computer's move
            game.mouse(button, mouseX, mouseY);
        } else {
            // if in the memu level / end level: display instruction / result
            // update shaderprogram
            game.update();
            glClear(GL_COLOR_BUFFER_BIT);
            // render the game
            game.render();
            SDL_GL_SwapWindow(displayWindow);
        }

    }

    // c.determineWinner();
    std::cout << "------------End of Game------------" << std::endl;

    SDL_Quit();


}
