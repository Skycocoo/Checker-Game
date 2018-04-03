//
//  main.cpp
//  CS4613 Checker
//
//  Created by Yuxi Luo on 03/04/2018.
//  Copyright © 2018 Yuxi Luo. All rights reserved.
//

#include <iostream>
#include <vector>

using namespace std;


#define HUSS 2
#define COMP 1

class Board{
public:
    int size = 6;
    vector<vector<int>> b;

    Board() {
        b.push_back(vector<int> {0, 1, 0, 1, 0, 1});
        b.push_back(vector<int> {1, 0, 1, 0, 1, 0});
        b.push_back(vector<int> {0, 0, 0, 0, 0, 0});
        b.push_back(vector<int> {0, 0, 0, 0, 0, 0});
        b.push_back(vector<int> {0, 2, 0, 2, 0, 2});
        b.push_back(vector<int> {2, 0, 2, 0, 2, 0});
    }
};

ostream& operator<<(ostream& os, const Board& b){
    for (size_t i = 0; i < b.b.size(); i++){
        for (size_t j = 0; j < b.b[i].size(); j++){
            os << b.b[i][j] << " ";
        }
        os << endl;
    }
    return os;
}


class Search{
public:

private:

};


class Checker{
public:
    Board board;

    
    Checker(): numB(6), numW(6){}

    void checkState(){
        // check the state of the game

        // whether there is legal move
        // & the number of black and white checkers
    }

    void move(int origx, int origy, int x, int y){
        // move a checker forom (origx, origy) to (x, y)
    }

    void play(){
        // determine the first / second one to make the move for each turn

        // while: input for human player & alpha-beta for computer
        // call checkState() for every white iteration

        // determine the winner / draw
    }

private:

    // black: human player
    int numB;
    // white: computer player
    int numW;
};



int main() {
    Checker c;
    cout << c.board;
}
