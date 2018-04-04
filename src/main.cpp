//
//  main.cpp
//  CS4613 Checker
//
//  Created by Yuxi Luo on 03/04/2018.
//  Copyright © 2018 Yuxi Luo. All rights reserved.
//

#include <iostream>
#include <vector>
#include "extern/Board.h"

using namespace std;



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
