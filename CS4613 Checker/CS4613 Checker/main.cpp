//
//  main.cpp
//  CS4613 Checker
//
//  Created by Yuxi Luo on 03/04/2018.
//  Copyright © 2018 Yuxi Luo. All rights reserved.
//

#include <iostream>

using namespace std;


class Board{
public:
    int[6][6] board;
}

class Search{
public:
    
private:

}


class Checker{
public:
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
    Board b;

    // black: human player
    int numB;
    // white: computer player
    int numW;
}



int main() {

}
