// Created by Yuxi Luo, April 2018
// For CS4613 Artificial Intelligence; Checker Game project

#ifndef MOVE_H
#define MOVE_H

#include <iostream>
#include <vector>

class Board;
class Checker;

// struct Point:
// an abstraction of the position of the checker
struct Point{
    int x;
    int y;
    // constructor
    Point(int x = -1, int y = -1);
    // convert to boolean if necessary
    explicit operator bool() const;
    // update position
    void update(int x, int y);
};

// standard output for Point
std::ostream& operator<<(std::ostream& os, const Point& p);


// class Move:
// an abstraction of move (original position -> target position)
class Move{
    // allow accessing private fields by Search
    friend class Search;
    friend class Checker;
    // standard output for Move
    friend std::ostream& operator<<(std::ostream& os, const Move& move);
public:
    Point cur;

    // constructor
    Move(int x, int y, const Board& board, int type);
    // convert to boolean if necessary
    explicit operator bool() const;

    // check selected position
    bool select(int x, int y) const;
    // check target position
    bool checkMove(int targx, int targy) const;
    // clear up this checker
    void clearMove();

    // update availability
    void updateMove();
    // update position
    void updatePos(int x, int y);
    // update board
    void updateBoard(const Board* board);

    // if the checker has capture move
    bool isCapture() const;
    // if the checker has regular move
    bool isRegular() const;

    // get the number of available moves
    int getMoves() const;
    // if this checker is captured
    bool getCap() const;

    // capture this checker
    void captured();
    // uncapture this checker
    void uncaptured();

private:
    const Board* board;
    // store the type of current checker
    bool isHuman;
    // indicate captured or not
    bool isCaptured;

    // index 0: left, index 1: right
    std::vector<Point> capture;
    std::vector<Point> regular;
};

#endif
