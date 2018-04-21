// Created by Yuxi Luo, April 2018
// For CS4613 Artificial Intelligence; Checker Game project

#ifndef AVAMOVES_H
#define AVAMOVES_H

#include <iostream>
#include <vector>

class Board;
#include "Move.h"

// class AvaMoves: an abstraction of available moves for human / computer
class AvaMoves{
    friend class Search;
    // standard output for AvaMoves
    friend std::ostream& operator<<(std::ostream& os, const AvaMoves& h);
public:
    // constructor
    AvaMoves(const Board& board, int type);

    // update checker board for human / computer
    void updateBoard(const Board& board);
    // update each move for availability
    void updateMoves();

    // select checker to be moved
    bool select(int x, int y, bool output = true);
    // check the target position for selected checker
    bool checkMove(int targX, int targY);
    // the corresponding checker captured by enemy
    int captured(int x, int y);

    // ------------ helper function for Search ------------ //
    // because Search performs entirely on index
    // manipulate correct indexing is necessary

    // move the checker by index
    bool superMove(int index, int targX, int targY);
    // reset the checker by index
    void reset(int index, int x, int y);
    // reset the captured checker by index
    void resetCaptured(int index);

    // ------------ end of helper functions ------------ //

    // there is available capture move
    bool avaCapture() const;
    // number of available moves for current player
    int avaMoves() const;

    // for heuristics
    int distance() const;

private:
    // type of current player (human / computer)
    int type;
    // current index for selection
    int cur;

    const Board* board;
    std::vector<Move> moves;
};

#endif
