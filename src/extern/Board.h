#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>

#define HUSS 2
#define COMP 1


class Board{
    // friend std::ostream& operator<<(std::ostream& os, const Board& b);
public:
    std::vector<std::vector<int>> b;

    Board(): numH(6), numC(6) {
        b.push_back(std::vector<int> {0, 1, 0, 1, 0, 1});
        b.push_back(std::vector<int> {1, 0, 1, 0, 1, 0});
        b.push_back(std::vector<int> {0, 0, 0, 0, 0, 0});
        b.push_back(std::vector<int> {0, 0, 0, 0, 0, 0});
        b.push_back(std::vector<int> {0, 2, 0, 2, 0, 2});
        b.push_back(std::vector<int> {2, 0, 2, 0, 2, 0});
    }

    void updateCount(){
        numH = 0, numC = 0;
        for (size_t i = 0; i < b.size(); i++){
            for (size_t j = 0; j < b[i].size(); j++){
                if (b[i][j] == HUSS) ++numH;
                if (b[i][j] == COMP) ++numC;
            }
        }
    }

    bool endState() const {
        return (numH == 0 || numC == 0);
    }

private:
    // int size = 6;
    // black: human player
    int numH;
    // white: computer player
    int numC;
};

std::ostream& operator<<(std::ostream& os, const Board& b){
    for (size_t i = 0; i < b.b.size()+1; i++){
        for (size_t j = 0; j < b.b[0].size()+1; j++){
            if (i == 0 || j == 0){
                if (i == 0 && j == 0) os << "| ";
                else if (i == 0) os << "|" << j-1;
                else if (j == 0) os << "|" << i-1;
            } else {
                if (b.b[i-1][j-1] == HUSS) os << "|B";
                else if (b.b[i-1][j-1] == COMP) os << "|W";
                else os << "| ";
            }
        }
        os << "|" << std::endl;
    }
    return os;
};

#endif
