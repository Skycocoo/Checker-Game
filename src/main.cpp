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

class Checker{
public:
    Checker(): board(), human(board, HUSS), comp(board, COMP){}

    bool endState() const {
        // check if the game is ended

        // NEED: whether there is any legal move for both players
        return board.endState() || human.avaMoves() == 0;

    }

    void move(int x, int y, int targx, int targy, int type){
        // move a checker forom (x, y) to (targx, targy)
        board.b[x][y] = 0;

        // if capture move
        if (abs(targy - y) == 2){
            if (type == HUSS){
                if (y - targy > 0) board.b[x - 1][y - 1] = 0;
                else board.b[x - 1][y + 1] = 0;
            } else {
                // this is moves for computer
            }

            board.updateCount();
        }
        board.b[targx][targy] = type;
    }

    void humanTurn(){
        cout << "Human turn" << endl << human;

        cout << "Please select the checker in \'x y\' format" << endl;
        bool select = false;
        int x = 0, y = 0;
        while (!select){
            cin >> x >> y;
            if ((select = human.select(x, y))){
                cout << "Please choose the location to move in \'x y\' format" << endl;
                bool target = false;
                int targx = 0, targy = 0;
                while (!target){
                    cin >> targx >> targy;
                    // make the move
                    if ((target = human.checkMove(targx, targy))) move(x, y, targx, targy, HUSS);
                    else cout << "Not a legal target location; please input correct locaion" << endl;
                }
            } else cout << "Not a legal checker to be moved; please correct location" << endl;
        }

        cout << board;
        update();

    }

    void computerTurn(){
        cout << "Computer turn" << endl;
        cout << comp;
        cout << board;
        update();
    }

    void update(){
        // remember to update moves for each turn
        human.updateMoves();
        comp.updateMoves();
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
    AvaMoves human;
    AvaMoves comp;
};



int main() {
    Checker c;
    c.play();
}
