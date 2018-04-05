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
    friend std::ostream& operator<<(std::ostream& os, const ComputerMoves& c){
        return os;
    }
public:
    ComputerMoves(const Board& board): type(COMP), board(&board) {
        for (size_t i = 0; i < this->board->b.size(); i++){
            for (size_t j = 0; j < this->board->b[i].size(); j++){
                if (this->board->b[i][j] == type){
                    moves.push_back(Move(i, j, board, type));
                }
            }
        }
        updateMoves();
    }

    void updateMoves(){
        for (size_t i = 0; i < moves.size(); i++){
            moves[i].updateMove();
        }

        // // only allow capture move for this checker
        // if (avaCapture()){
        //     for (size_t i = 0; i < moves.size(); i++){
        //         if (!moves[i].isCapture()) moves[i].clearMove();
        //     }
        // }
    }


private:
    int type;
    const Board* board;
    std::vector<Move> moves;
};


class Checker{
public:
    Checker(): board(), human(board), comp(board){}

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
    ComputerMoves comp;
};



int main() {
    Checker c;
    c.play();
}
