// Yuxi Luo (yl4217), February 26, 2018
// CS3113 Game Programming

#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Checker.h"

class GameState{
public:
    GameState();
    void init();

    void update();
    void render();

    void displayMainMenu();
    void displayLevel();
    void displayOver();

private:
    Text text;
    Checker c;

    Object title;

};

#endif /* GameState_h */
