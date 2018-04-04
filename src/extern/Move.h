#ifndef MOVE_H
#define MOVE_H

#include <iostream>
#include "Board.h"

struct Point{
    int x;
    int y;
    Point(int x = -1, int y = -1);

    void update(int x, int y);
    explicit operator bool() const;
};

class Move{
    friend std::ostream& operator<<(std::ostream& os, const Move& move);
public:
    Move(int x, int y, const Board& board, int type);

    bool isCapture() const;
    bool isRegular() const;

    // both capture move & regular move
    void updateMove();

private:
    const Board* board;
    bool isHuman;

    Point cur;
    Point left1, right1, left2, right2;
};

#endif