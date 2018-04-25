// Yuxi Luo (yl4217), February 26, 2018
// CS3113 Game Programming

#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Checker.h"

class GameState{
public:
    GameState();

    void setOrder(int o);
    void setDiff(int d);
    void mouse(uint& button, int mouseX, int mouseY, bool& done);

    void update();
    void render();

    void displayMainMenu();
    void displayLevel();
    void displayOver();

private:
    Text text;
    Checker c;

    Object title;

    int order;
    int diff;

};

#endif /* GameState_h */
