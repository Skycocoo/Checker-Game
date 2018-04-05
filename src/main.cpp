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

class HumanMoves{
    friend ostream& operator<<(ostream& os, const HumanMoves& h){
        os << "You have " << h.avaMoves() << " avaliable moves\n";
        for (size_t i = 0; i < h.moves.size(); i++){
            if (h.moves[i]) os << h.moves[i];
        }
        os << "\n";
        return os;
    }

public:
    HumanMoves(const Board& board): type(HUSS), cur(-1), board(&board){
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

        // only allow capture move for this checker
        if (avaCapture()){
            for (size_t i = 0; i < moves.size(); i++){
                if (!moves[i].isCapture()) moves[i].clearMove();
            }
        }
    }


    bool select(int x, int y) {
        cur = -1;
        for (size_t i = 0; i < moves.size(); i++){
            if (moves[i].select(x, y) && moves[i]){
                cur = i;
                cout << "You selected " << moves[cur];
                return true;
            }
        }
        // cout << "Not a legal selection" << endl;
        return false;
    }

    bool checkMove(int targx, int targy) const {
        return moves[cur].checkMove(targx, targy);
    }



    bool avaCapture() const {
        for (size_t i = 0; i < moves.size(); i++){
            if (moves[i].isCapture()) return true;
        }
        return false;
    }

    int avaMoves() const {
        int count = 0;
        for (size_t i = 0; i < moves.size(); i++){
            if (moves[i]) ++count;
        }
        return count;
    }

private:
    int type;
    int cur;

    const Board* board;
    vector<Move> moves;
};

class ComputerMoves{

};



class Checker{
public:
    Checker(): board(), human(board){}

    bool endState() const {
        // check if the game is ended

        // NEED: whether there is any legal move for both players
        return board.endState();

    }

    void move(int x, int y, int targx, int targy, int type){
        // move a checker forom (x, y) to (targx, targy)
        board.b[x][y] = 0;

        // if capture move
        if (abs(targy - y) == 2 && type == HUSS){
            if (y - targy > 0) board.b[targx - 1][targy - 1] = 0;
            else board.b[targx - 1][targy + 1] = 0;

            board.updateCount();
        }
        board.b[targx][targy] = type;
    }

    // // if there is no capture move
    // // moves contains the possible target position for (origx, origy)
    // bool humanRegular(int origx, int origy){
    //     // return bool to indicate whether there is a legal move for (origx, origy)
    //     // any legal move will be stored in moves
    //     humanMoves.clear();
    //
    //     if (origx >= 0 && origy >= 0 && origx < board.b.size() && origy < board.b[origx].size()){
    //         if (board.b[origx][origy] != HUSS) return false;
    //
    //         int newx = origx - 1, lefty = origy - 1, righty = origy + 1;
    //         if (newx >= 0){
    //             if (lefty >= 0 && board.b[newx][lefty] == 0) humanMoves.insert(humanMoves.end(), {newx, lefty});
    //             if (righty < board.b[newx].size() && board.b[newx][righty] == 0) humanMoves.insert(humanMoves.end(), {newx, righty});
    //             if (humanMoves.size() > 0) return true;
    //         }
    //     }
    //     return false;
    // }
    //
    // // if there is capture move
    // // moves contains the possible target position for each (origx, origy)
    // bool humanCapture(){
    //     humanMoves.clear();
    //     humanCapLoc.clear();
    //
    //     for (int i = 0; i < board.b.size(); i++){
    //         for (int j = 0; j < board.b[i].size(); j++){
    //             if (board.b[i][j] == HUSS){
    //                 int newx = i - 2, lefty = j - 2, righty = j + 2;
    //                 bool found = false;
    //                 if (newx >= 0){
    //                     if (lefty >= 0 && board.b[newx][lefty] == 0 && board.b[newx + 1][lefty + 1] == COMP){
    //                         humanMoves.insert(humanMoves.end(), {newx, lefty});
    //                         found = true;
    //                     }
    //                     if (righty < board.b[newx].size() && board.b[newx][righty] == 0 && board.b[newx + 1][righty - 1] == COMP){
    //                         humanMoves.insert(humanMoves.end(), {newx, righty});
    //                         found = true;
    //                     }
    //                     if (found) humanCapLoc.insert(humanCapLoc.end(), {i, j});
    //                 }
    //             }
    //         }
    //     }
    //
    //     if (humanMoves.size() > 0) return true;
    //     else return false;
    // }

    // // if there is no capture move
    // void humanMove(int origx, int origy){
    //     // check legal moves for origx and origy
    //     // provide legal moves for human
    //
    //
    //     // read in the move
    //     // move a checker forom (origx, origy) to (x, y)
    //     // display the result
    // }


    // NEED
    // for all functions: if return false: should do a while until it returns true


    void humanTurn(){
        cout << "Human turn" << endl;

        cout << human;


        // if (humanCapture()){
        //     cout << "You should take a capture move fort his turn" << endl;
        //     // displayMoves();
        //
        // }


        // check if there is capture move
        // provide avaliable capture moves

        // if not: provide all avaliable slots that could be moved?

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
