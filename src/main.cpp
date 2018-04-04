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
    Checker(){}

    bool endState() const {
        // check if the game is ended

        // NEED: whether there is any legal move for both players
        return board.endState();

    }

    void move(int origx, int origy, int x, int y, int type){
        // move a checker forom (origx, origy) to (x, y)
        board.b[origx][origy] = 0;

        // if capture move
        if (abs(y - origy) == 2 && type == HUSS){
            if (origy - y > 0) board.b[x - 1][y - 1] = 0;
            else board.b[x - 1][y + 1] = 0;

            board.updateCount();
        }

        board.b[x][y] = type;
    }

    // if there is no capture move
    // moves contains the possible target position for (origx, origy)
    bool humanRegular(int origx, int origy, vector<int>& moves){
        // return bool to indicate whether there is a legal move for (origx, origy)
        // any legal move will be stored in moves
        moves.clear();

        if (origx >= 0 && origy >= 0 && origx < board.b.size() && origy < board.b[origx].size()){
            if (board.b[origx][origy] != HUSS) return false;

            int newx = origx - 1, lefty = origy - 1, righty = origy + 1;
            if (newx >= 0){
                if (lefty >= 0 && board.b[newx][lefty] == 0) moves.insert(moves.end(), {newx, lefty});
                if (righty < board.b[newx].size() && board.b[newx][righty] == 0) moves.insert(moves.end(), {newx, righty});
                if (moves.size() > 0) return true;
            }
        }
        return false;
    }

    // if there is capture move
    // moves contains the possible target position for (origx, origy)
    bool humanCapture(vector<int>& moves){
        moves.clear();

        for (int i = 0; i < board.b.size(); i++){
            for (int j = 0; j < board.b[i].size(); j++){
                if (board.b[i][j] == HUSS){
                    int newx = i - 2, lefty = j - 2, righty = j + 2;
                    if (newx >= 0){
                        if (lefty >= 0 && board.b[newx][lefty] == 0 && board.b[newx + 1][lefty + 1] == COMP){
                            moves.insert(moves.end(), {newx, lefty});
                        }
                        if (righty < board.b[newx].size() && board.b[newx][righty] == 0 && board.b[newx + 1][righty - 1] == COMP){
                            moves.insert(moves.end(), {newx, righty});
                        }
                    }
                }
            }
        }

        if (moves.size() > 0) return true;
        else return false;
    }

    // if there is no capture move
    void humanMove(int origx, int origy){
        // check legal moves for origx and origy
        // provide legal moves for human


        // read in the move
        // move a checker forom (origx, origy) to (x, y)
        // display the result
    }


    // NEED
    // for all functions: if return false: should do a while until it returns true


    void human(){
        cout << "Human turn" << endl;
        // check if there is capture move
        // provide avaliable capture moves

        // if not: provide all avaliable slots that could be moved?

        cout << "Please indicate the checker to be moved in the format of \'x, y\'" << endl;
        bool select = false;
        int x = 0, y = 0;
        char comma;
        while (!select){
            cin >> x >> comma >> y;
            if ((select = humanRegular(x, y, humanMoves))){
                displayMoves();
                cout << "Please indicate where to move the checker in the format of \'x, y\'" << endl;
                bool target = false;
                int targx = 0, targy = 0;
                while (!target){
                    cin >> targx >> comma >> targy;
                    // make the move
                    if ((target = checkMove(targx, targy))) move(x, y, targx, targy, HUSS);
                    else cout << "Not a legal target location; please indicate again" << endl;
                }
            } else cout << "Not a legal checker could be moved; please indicate again" << endl;
        }

        cout << board;
    }

    void computer(){

    }

    void play(){
        // determine the first / second one to make the move for each turn

        // while: input for human player & alpha-beta for computer
        // call checkState() for every white iteration

        // determine the winner / draw

        cout << "Checker Game, 6 * 6 board" << endl;
        cout << "The Board is Row-Major; \ne.g. the bottom-left W checker's position is (5, 0)" << endl;
        cout << board;

        // cout << "input 1 to take first move; 2 to take second move" << endl;
        int flag = 1;
        // cin >> flag;
        if (flag == 2) computer();
        while (!endState()){
            human();
            computer();
        }


    }

private:
    Board board;
    vector<int> humanMoves;

    void displayMoves() const {
        cout << "You have a total of " << humanMoves.size() / 2 << " moves" << endl;
        for (size_t i = 0; i < humanMoves.size() / 2; i++){
            cout << "\tThe " << i+1 << " move: (" << humanMoves[2 * i] << ", " << humanMoves[2 * i + 1] << ")" << endl;
        }
    }

    bool checkMove(int x, int y) const {
        cout << x << " " << y << endl;
        for (size_t i = 0; i < humanMoves.size() / 2; i++){
            if (humanMoves[2 * i] == x && humanMoves[2 * i + 1] == y) return true;
        }
        return false;
    }
};



int main() {
    Checker c;
    c.play();
}
