#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>

#define HUSS 2
#define COMP 1


class Board{
public:
    int size = 6;
    std::vector<std::vector<int>> b;

    Board() {
        b.push_back(std::vector<int> {0, 1, 0, 1, 0, 1});
        b.push_back(std::vector<int> {1, 0, 1, 0, 1, 0});
        b.push_back(std::vector<int> {0, 0, 0, 0, 0, 0});
        b.push_back(std::vector<int> {0, 0, 0, 0, 0, 0});
        b.push_back(std::vector<int> {0, 2, 0, 2, 0, 2});
        b.push_back(std::vector<int> {2, 0, 2, 0, 2, 0});
    }
};

std::ostream& operator<<(std::ostream& os, const Board& b){
    for (size_t i = 0; i < b.b.size()+1; i++){
        for (size_t j = 0; j < b.b[0].size()+1; j++){
            if (i == 0 || j == 0){
                if (i == 0 && j == 0) os << "| ";
                else if (i == 0) os << "|" << j-1;
                else if (j == 0) os << "|" << i-1;
            } else {
                if (b.b[i-1][j-1] == HUSS) os << "|H";
                else if (b.b[i-1][j-1] == COMP) os << "|C";
                else os << "| ";
            }
        }
        os << "|" << std::endl;
    }
    return os;
}

#endif
