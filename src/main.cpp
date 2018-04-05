//
//  main.cpp
//  CS4613 Checker
//
//  Created by Yuxi Luo on 03/04/2018.
//  Copyright © 2018 Yuxi Luo. All rights reserved.
//

#include <iostream>
#include <vector>
#include <chrono>

#include "includes.h"

using namespace std;

class Search{
public:
    Search(const AvaMoves& human, const AvaMoves& comp, const Board& board):
    human(human), comp(comp), board(board){
        human.updateBoard(board);
        comp.updateBoard(board);

        // unnecessary?
        updateMoves();
    }

    void updateMoves(){
        // remember to update moves for each turn
        human.updateMoves();
        comp.updateMoves();
    }

    void updateBoard(const Board& board){
        for (size_t i = 0; i < this->board->b.size(); i++){
            for (size_t j = 0; j < this->board->b[i].size(); j++){
                this->board->b[i][j] = board.b[i][j];
            }
        }
        updateMoves();
    }

    Move iterativeDeep(int maxDepth = 100){
        Move result(-1, -1, board, COMP);

        auto start = std::chrono::system_clock::now();
        for (int i = 1; i < maxDepth; i++){

            // alphaBeta();
            // if the utility value for the returned move is larger
            // change the current result to the returned move


            // if the duration >= 14
            // need to satisfy the duration requirement (within 15 seconds)
            auto end = std::chrono::system_clock::now();
            std::chrono::duration<double> elapsed = end - start;
            if (elapsed.count() >= 14) break;
        }
    }

    // return the action or the estimated value?
    void alphaBeta(){
        // parameters: depth
        maxVal();
    }

    // maxVal: for COMP player
    void maxVal(){
        // parameters: alpha, beta, depth

        // if terminalState: return utility
        // if depth == 0: return evaluated value
        // if times up: return 0? (didnt finish searching for this level)

        // initial v = -6

        // for each avaliable moves for computer:

            // // need to remember the move & restore the board after iteration
            // v = max(v, minVal(updated board, alpha, beta))
            // if v >= beta: return v (pruned)
            // alpha = max(alpha, v)

        // return v?

    }

    // minVal: for HUSS player
    void minVal(){
        // parameters: alpha, beta, depth

        // if terminalState: return utility
        // if depth == 0: return evaluated value
        // if times up: return 0? (didnt finish searching for this level)

        // initial v = 6

        // for each avaliable moves for human:
            // // need to remember the move & restore the board after iteration
            // v = min(v, maxVal(updated board, alpha, beta))
            // if v <= alpha: return v (pruned)
            // beta = min(beta, v)

        // return v?
    }

    // heuristics to estimate the expected utility
    void evaluation(){
        // parameters: player type, (board)
        // should be in range [-6, 6]

        // features:
        // number of avaliable checkers for current player
        // for each avalible checker: proximity to the other end
        // number of captured enemy

        // negative: number of enemies
        // negative: for each avalible checker of enemy: proximity to the other end

        // linear weighted sum of features
    }

    // utility value for terminal state: range [-6, 6] = numC - numH
    void utility(){
        // numC - numH
    }



    void terminalState() const {
        return board.terminalState() || (human.avaMoves() == 0 && comp.avaMoves() == 0);
    }

private:
    AvaMoves human;
    AvaMoves comp;
    Board board;
};


class Checker{
public:
    Checker(): board(), human(board, HUSS), comp(board, COMP){}

    bool terminalState() const {
        // check if the game is ended

        // NEED: whether there is any legal move for both players
        return board.terminalState() || (human.avaMoves() == 0 && comp.avaMoves() == 0);

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
        updateMoves();

    }

    void computerTurn(){
        cout << "Computer turn" << endl;
        // cout << comp;
        cout << board;
        updateMoves();
    }

    void updateMoves(){
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
        while (!terminalState()){
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
