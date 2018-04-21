// Created by Yuxi Luo, April 2018
// For CS4613 Artificial Intelligence; Checker Game project

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
    explicit operator bool() const;

    void update(int x, int y);
};

std::ostream& operator<<(std::ostream& os, const Point& p);

class Move{
    friend class Search;
    friend std::ostream& operator<<(std::ostream& os, const Move& move);
public:
    Point cur;
    Move(int x, int y, const Board& board, int type);
    explicit operator bool() const;

    bool select(int x, int y) const;
    bool checkMove(int targx, int targy) const;
    void clearMove();

    void updateMove();
    void updatePos(int x, int y);
    void updateBoard(const Board* board);

    bool isCapture() const;
    bool isRegular() const;

    int getMoves() const;
    bool getCap() const;

    void captured();
    void uncaptured();

private:
    const Board* board;
    bool isHuman;
    bool isCaptured;

    Point left1, right1, left2, right2;

    // index 0: left, index 1: right
    std::vector<Point> capture;
    std::vector<Point> regular;
};

#endif
