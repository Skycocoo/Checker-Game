// Created by Yuxi Luo, April 2018
// For CS4613 Artificial Intelligence; Checker Game project

#include "../extern/Board.h"

// standard output for Board
// print out the checker board
// return: std::ostream&
std::ostream& operator<<(std::ostream& os, const Board& b){
    os << "Board: \n";
    for (size_t i = 0; i < b.b.size()+1; i++){
        for (size_t j = 0; j < b.b[0].size()+1; j++){
            if (i == 0 || j == 0){
                if (i == 0 && j == 0) os << "| ";
                else if (i == 0) os << "|" << j-1;
                else if (j == 0) os << "|" << i-1;
            } else {
                // B: black: human
                if (b.b[i-1][j-1] == HUSS) os << "|B";
                // W: white: computer
                else if (b.b[i-1][j-1] == COMP) os << "|W";
                else os << "| ";
            }
        }
        os << "|\n";
    }

    os << "Number of human: " << b.numH;
    os << "\nNumber of computer: " << b.numC;
    os << "\n\n";

    return os;
};


// constructor
Board::Board(): numH(6), numC(6) {
    b.push_back(std::vector<int> {0, 1, 0, 1, 0, 1});
    b.push_back(std::vector<int> {1, 0, 1, 0, 1, 0});
    b.push_back(std::vector<int> {0, 0, 0, 0, 0, 0});
    b.push_back(std::vector<int> {0, 0, 0, 0, 0, 0});
    b.push_back(std::vector<int> {0, 2, 0, 2, 0, 2});
    b.push_back(std::vector<int> {2, 0, 2, 0, 2, 0});
}


// check terminal state for board
// return: bool: terminal state or not
bool Board::terminalState() const {
    // if one of the player does not have any checker
    return (numH == 0 || numC == 0);
}
