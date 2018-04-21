// Created by Yuxi Luo, April 2018
// For CS4613 Artificial Intelligence; Checker Game project

#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>

#define HUSS 2
#define COMP 1


class Board{
    friend std::ostream& operator<<(std::ostream& os, const Board& b);
public:
    std::vector<std::vector<int>> b;

    // black: human player
    int numH;
    // white: computer player
    int numC;

    Board();
    void updateCount();
    bool terminalState() const;

private:
    // int size = 6;

};

#endif
