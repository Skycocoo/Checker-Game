// Created by Yuxi Luo, April 2018
// For CS4613 Artificial Intelligence; Checker Game project

#ifndef CHECKER_H
#define CHECKER_H

#include "Board.h"
#include "AvaMoves.h"
#include "Search.h"

#include "Object.h"
#include "Text.h"

// class Checker:
// an abstraction of the game
class Checker{
public:
    // constructor
    Checker();

    // update shaders for render
    void update();
    // render the checker game
    void render();

    // set the difficulty mode
    void setDiff(int d) const;
    // convert from mouse position to board position
    void convertMouse(int& x, int& y) const;

    // if human does not have available move
    bool humanAva() const;

    // human turn
    bool humanTurn(int x, int y, bool& done);
    // computer turn
    void computerTurn();

    // update availability for both human and computer (and board)
    void updateMoves();

    // move checker for board
    void move(int x, int y, int targx, int targy, int type);

    // play the game
    void play();
    // check the terminal state for the game
    bool terminalState() const;

    // determine the winner of the game
    int determineWinner() const;

private:
    Board board;
    AvaMoves human;
    AvaMoves comp;

    Search search;

    Object background;

    Object checkerH;
    Object checkerH2;
    Object checkerC;

    Text text;

    bool humanSelect;
};


#endif
