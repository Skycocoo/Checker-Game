//
//  main.cpp
//  CS4613 Checker
//
//  Created by Yuxi Luo on 03/04/2018.
//  Copyright © 2018 Yuxi Luo. All rights reserved.
//

#include <iostream>
#include <vector>

#include "includes.h"

using namespace std;

class ComputerMoves{

};


class Checker{
public:
    Checker(): board(), human(board){}

    bool endState() const {
        // check if the game is ended

        // NEED: whether there is any legal move for both players
        return board.endState() || human.avaMoves() == 0;

    }

    void move(int x, int y, int targx, int targy, int type){
        // move a checker forom (x, y) to (targx, targy)
        board.b[x][y] = 0;

        // if capture move
        if (abs(targy - y) == 2 && type == HUSS){
            if (y - targy > 0) board.b[x - 1][y - 1] = 0;
            else board.b[x - 1][y + 1] = 0;

            board.updateCount();
        }
        board.b[targx][targy] = type;
    }

    void humanTurn(){
        cout << "Human turn" << endl << human;

        cout << "Please indicate the checker to be moved in the format of \'x y\'" << endl;
        bool select = false;
        int x = 0, y = 0;
        while (!select){
            cin >> x >> y;
            if ((select = human.select(x, y))){
                cout << "Please indicate where to move the checker in the format of \'x y\'" << endl;
                bool target = false;
                int targx = 0, targy = 0;
                while (!target){
                    cin >> targx >> targy;
                    // make the move
                    if ((target = human.checkMove(targx, targy))) move(x, y, targx, targy, HUSS);
                    else cout << "Not a legal target location; please indicate again" << endl;
                }
            } else cout << "Not a legal checker could be moved; please indicate again" << endl;
        }

        cout << board;

        // remember to update moves for each turn
        human.updateMoves();
    }

    void computerTurn(){
        cout << "Computer turn" << endl;
    }

    void play(){
        // determine the first / second one to make the move for each turn

        // while: input for human player & alpha-beta for computer
        // call checkState() for every white iteration

        // determine the winner / draw

        cout << "Checker Game, 6 * 6 board" << endl;
        cout << "The Board is Row-Major; \ne.g. the bottom-left W checker's position is (5, 0)" << endl;
        cout << endl;
        cout << board;

        // cout << "input 1 to take first move; 2 to take second move" << endl;
        int flag = 1;
        // cin >> flag;
        if (flag == 2) computerTurn();
        while (!endState()){
            humanTurn();
            computerTurn();
        }


    }

private:
    Board board;
    HumanMoves human;

    // vector<int> humanMoves;
    // vector<int> humanCapLoc;
    //
    // void displayMoves() const {
    //     cout << "You have a total of " << humanMoves.size() / 2 << " moves" << endl;
    //     for (size_t i = 0; i < humanMoves.size() / 2; i++){
    //         cout << "\tThe " << i+1 << " move: (" << humanMoves[2 * i] << ", " << humanMoves[2 * i + 1] << ")" << endl;
    //     }
    // }
    //
    // // keep each location for each location?
    // void displayCap() const {
    //
    // }
    //
    // bool checkMove(int x, int y) const {
    //     cout << x << " " << y << endl;
    //     for (size_t i = 0; i < humanMoves.size() / 2; i++){
    //         if (humanMoves[2 * i] == x && humanMoves[2 * i + 1] == y) return true;
    //     }
    //     return false;
    // }
};



int main() {
    Checker c;
    c.play();
}
