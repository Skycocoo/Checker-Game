#ifndef AVAMOVES_H
#define AVAMOVES_H

#include <iostream>
#include <vector>

class Board;
// class Move;
#include "Move.h"

class AvaMoves{
    friend class Search;
    // provide all avaliable moves
    friend std::ostream& operator<<(std::ostream& os, const AvaMoves& h);
public:
    AvaMoves(const Board& board, int type);
    void updateBoard(const Board& board);
    void updateMoves();

    bool select(int x, int y, bool flag = true);
    bool checkMove(int targx, int targy);
    void reset(int x, int y);

    bool avaCapture() const;
    int avaMoves() const;

    // for heuristics
    int proximity() const;
private:
    int type;
    int cur;

    const Board* board;
    std::vector<Move> moves;
};

#endif
