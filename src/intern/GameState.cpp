// Yuxi Luo (yl4217), February 26, 2018
// CS3113 Game Programming

#include "GameState.h"
#include "setUp.h"


enum GameMode {STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER};
extern GameMode mode;
extern float screenWidth;
extern float screenHeight;

extern ShaderProgram textured;

GameState::GameState(){
    GLuint font;
    textured = setTextured("font.png", font);
    text = Text(&textured, font);

    GLuint ti;
    textured = setTextured("title.png", ti);
    title = Object(&textured, ti);
    title.setShape(glm::vec3(screenWidth, screenHeight, 0));
    title.setScale(2 * screenHeight);
}


void GameState::update(){
    switch (mode){
        case STATE_MAIN_MENU:
            title.update();
            break;
        case STATE_GAME_LEVEL:
            title.update();
            c.update();
            break;
        case STATE_GAME_OVER:
            title.update();
            break;
    }
}


void GameState::render(){
    switch (mode){
        case STATE_MAIN_MENU:
            displayMainMenu();
            break;
        case STATE_GAME_LEVEL:
            displayLevel();
            break;
        case STATE_GAME_OVER:
            displayOver();
            break;
    }

}


void GameState::displayMainMenu(){
    title.render();
    text.render("Checker Game", 1, 2, 0, 3.5);
    // text.render("game has a 6*6 board", 0.5, 1, 0, 2);
    // text.render("checkers can move diagonally", 0.5, 1, 0, 1);
    text.render("press 1: move first", 0.5, 1, 0, 2);
    text.render("press 2: move second", 0.5, 1, 0, 1);
    text.render("press a: easy mode", 0.5, 1, 0, -0.5);
    text.render("press b: medium mode", 0.5, 1, 0, -1.5);
    text.render("press c: hard mode", 0.5, 1, 0, -2.5);

    text.render("press Q to quit", 0.5, 1, 0, -4);
}

void GameState::displayLevel(){
    c.render();

}

void GameState::displayOver(){
    text.render("Game Over", 1, 2, 0, 1.5);
}
