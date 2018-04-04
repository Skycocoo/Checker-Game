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

    Board();
    void updateCount();
    bool endState() const;

private:
    // int size = 6;
    // black: human player
    int numH;
    // white: computer player
    int numC;
};

#endif
