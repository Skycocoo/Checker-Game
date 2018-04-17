#ifndef MOVE_H
#define MOVE_H

#include <iostream>
#include <vector>

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
    friend class Search;
    friend std::ostream& operator<<(std::ostream& os, const Move& move);
public:
    Point cur;
    Move(int x, int y, const Board& board, int type);

    void updateBoard(const Board* board);

    bool isCapture() const;
    bool isRegular() const;
    explicit operator bool() const;

    bool select(int x, int y) const;
    bool checkMove(int targx, int targy) const;

    void updatePos(int x, int y);

    // both capture move & regular move
    void updateMove();
    void clearMove();

private:
    const Board* board;
    bool isHuman;
    Point left1, right1, left2, right2;

    // index 0: left, index 1: right
    std::vector<Point> capture;
    std::vector<Point> regular;
};

#endif
