// Yuxi Luo (yl4217), February 26, 2018
// CS3113 Game Programming

#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Checker.h"

// class GameState:
// an abstraction of three levels of game
// manages the display of the game
class GameState{
public:
    // constructor
    GameState();

    // set the player to move first / second
    void setOrder(int o);
    // set the difficulty
    void setDiff(int d);
    // process the mouse input
    void mouse(uint& button, int mouseX, int mouseY);

    // update the shader to render
    void update();
    // render thr game
    void render();

    // render for menu level
    void displayMainMenu();
    
    // render for game level
    // void displayLevel

    // render for end level
    void displayOver();

private:
    Text text;
    Checker c;

    Object title;

    int order;
    int diff;
};

#endif /* GameState_h */
