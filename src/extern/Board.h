// Created by Yuxi Luo, April 2018
// For CS4613 Artificial Intelligence; Checker Game project

#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>

// represent human as 2
#define HUSS 2
// represent computer as 1
#define COMP 1

// class Board: an abstraction of checker board
class Board{
    // standard output for Board
    friend std::ostream& operator<<(std::ostream& os, const Board& b);
public:
    // Board: 2D vector
    std::vector<std::vector<int>> b;
    // number of human
    int numH;
    // number of computer
    int numC;

    // constructor
    Board();

    // check terminal state for board
    bool terminalState() const;

};

#endif
