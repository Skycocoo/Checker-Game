#ifndef MOVE_H
#define MOVE_H

#include <iostream>

// avoid forward-backward includes
class Board;

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

    bool select(int x, int y) const {
        return (x == cur.x && y == cur.y);
    }

    bool checkMove(int targx, int targy) const {
        return ((left1.x == targx && left1.y == targy) ||
            (left2.x == targx && left2.y == targy)     ||
            (right1.x == targx && right1.y == targy)   ||
            (right2.x == targx && right2.y == targy));
    }

    // both capture move & regular move
    void updateMove();
    void clearMove();
    explicit operator bool() const;
private:
    const Board* board;
    bool isHuman;

    Point cur;
    Point left1, right1, left2, right2;
};

#endif
