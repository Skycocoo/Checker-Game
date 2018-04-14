#ifndef CHECKER_H
#define CHECKER_H

#include "Board.h"
#include "AvaMoves.h"
#include "Search.h"


class Checker{
public:
    Checker();

    void move(int x, int y, int targx, int targy, int type);

    void humanTurn();
    void computerTurn();
    void updateMoves();

    void play();
    bool terminalState() const;

private:
    Board board;
    AvaMoves human;
    AvaMoves comp;

    Search search;
};


#endif
