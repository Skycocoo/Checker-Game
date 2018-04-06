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
#include <limits>

#include "includes.h"

using namespace std;

struct Result{
    int x, y, targX, targY;

    Result(int x, int y, int targX, int targY): x(x), y(y), targX(targX), targY(targY){}
    void update(int x, int y, int targX, int targY){
        this->x = x;
        this->y = y;
        this->targX = targX;
        this->targY = targY;
    }
    void update(const Result& r){
        this->x = r.x;
        this->y = r.y;
        this->targX = r.targX;
        this->targY = r.targY;
    }
};


class Search{
public:
    Search(const AvaMoves& human, const AvaMoves& comp, const Board& board):
    human(human), comp(comp), board(board){
        this->human.updateBoard(board);
        this->comp.updateBoard(board);

        // unnecessary?
        updateMoves();
    }

    void updateMoves(){
        // remember to update moves for each turn
        human.updateMoves();
        comp.updateMoves();
    }

    void updateBoard(const Board& board){
        for (size_t i = 0; i < this->board.b.size(); i++){
            for (size_t j = 0; j < this->board.b[i].size(); j++){
                this->board.b[i][j] = board.b[i][j];
            }
        }
        updateMoves();
    }

    Result iterativeDeep(int maxDepth = 100){
        Result fmove (-1, -1, -1, -1); // result of alphabeta (first max)
        float util = -std::numeric_limits<float>::max();

        start = std::chrono::system_clock::now();

        Result cmove (-1, -1, -1, -1);
        for (int i = 1; i < maxDepth; i++){
            float tempUtil = alphaBeta(cmove, i);

            // if the utility value for the returned move is larger
            // change the current result to the returned move
            if (tempUtil > util) {
                util = tempUtil;
                fmove.update(cmove);
            }
            // if the duration >= 14
            // need to satisfy the duration requirement (within 15 seconds)

            // auto end = std::chrono::system_clock::now();
            // std::chrono::duration<double> elapsed = end - start;
            // if (elapsed.count() >= 14) break;
        }
        return fmove;
    }

    // return the action or the estimated value?
    float alphaBeta(Result& fmove, int depth){
        // parameters: depth
        float alpha = -6, beta = 6;
        return maxVal(alpha, beta, fmove, depth);
    }

    // maxVal: for COMP player
    float maxVal(float alpha, float beta, Result& fmove, int depth){

        // edge cases

        // parameters: alpha, beta, depth
        if (terminalState()) return utility();

        // if depth == 0: return evaluated utility
        if (depth == 0) return eval(COMP);

        // if times up: evaluated utility
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        if (elapsed.count() >= 14) return eval(COMP);


        // main function

        float v = -6, tempv;
        Result cmove (-1, -1, -1, -1); // result of min
        bool capture = comp.avaCapture();
        for (size_t i = 0; i < comp.moves.size(); i++){
            int posX = comp.moves[i].cur.x, posY = comp.moves[i].cur.y;

            // capture move
            if (capture){
                for (size_t j = 0; j < comp.moves[i].capture.size(); j++){
                    if (comp.moves[i].capture[j]){
                        int targX = comp.moves[i].capture[j].x, targY = comp.moves[i].capture[j].y;
                        move(posX, posY, targX, targY, COMP);
                        tempv = minVal(alpha, beta, cmove, depth - 1);
                        reset(posX, posY, targX, targY, COMP);

                        if (tempv > v){
                            v = tempv;
                            fmove.update(posX, posY, targX, targY);
                        }
                        if (v >= beta) return v;
                        if (v > alpha) alpha = v;
                    }
                }

            } else {
                // regular move
                for (size_t j = 0; j < comp.moves[i].regular.size(); j++){
                    if (comp.moves[i].regular[j]){
                        int targX = comp.moves[i].regular[j].x, targY = comp.moves[i].regular[j].y;
                        move(posX, posY, targX, targY, COMP);
                        tempv = minVal(alpha, beta, cmove, depth - 1);
                        reset(posX, posY, targX, targY, COMP);

                        if (tempv > v){
                            v = tempv;
                            fmove.update(posX, posY, targX, targY);
                        }
                        if (v >= beta) return v;
                        if (v > alpha) alpha = v;
                    }
                }
            }
        }
        return v;
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

    // move from (x, y) to (targx, targy)
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
                if (y - targy > 0) board.b[x + 1][y - 1] = 0;
                else board.b[x + 1][y + 1] = 0;
            }
            board.updateCount();
        }
        board.b[targx][targy] = type;
    }

    // reset the move from (x, y) to (targx, targy) back to original
    void reset(int x, int y, int targx, int targy, int type){
        board.b[targx][targy] = 0;
        // if capture move
        if (abs(targy - y) == 2){
            if (type == HUSS){
                if (y - targy > 0) board.b[x - 1][y - 1] = COMP;
                else board.b[x - 1][y + 1] = COMP;
            } else {
                // this is moves for computer
                if (y - targy > 0) board.b[x + 1][y - 1] = HUSS;
                else board.b[x + 1][y + 1] = HUSS;
            }
            board.updateCount();
        }
        board.b[x][y] = type;
    }


    // heuristics to estimate the expected utility
    float eval(int type = HUSS){
        // parameters: player type, (board)
        // should be in range [-6, 6]

        // features:
        // [0, 6] number of avaliable checkers for current player
        int fea1 = human.avaMoves();
        if (type == COMP) fea1 = comp.avaMoves();

        // [0, 18] for each avalible checker: proximity to the other end
        int fea2 = human.proximity();
        if (type == COMP) fea2 = comp.proximity();

        // [-18, 0] negative: for each avalible checker of enemy: proximity to the other end
        int fea3 = -comp.proximity();
        if (type == COMP) fea3 = -human.proximity();

        // [-6, 0] negative: number of enemy
        int fea4 = -board.numC;
        if (type == COMP) fea4 = -board.numH;

        // linear weighted sum of features
        return (fea1 + fea2 + fea3 + fea4) / 24 * 6;
    }

    // utility value for terminal state: range [-6, 6] = numC - numH
    float utility(){
        return board.numC - board.numH;
    }



    bool terminalState() const {
        return board.terminalState() || (human.avaMoves() == 0 && comp.avaMoves() == 0);
    }

private:
    AvaMoves human;
    AvaMoves comp;
    Board board;

     std::chrono::system_clock::time_point start;
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
                if (y - targy > 0) board.b[x + 1][y - 1] = 0;
                else board.b[x + 1][y + 1] = 0;
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
        cout << comp;
        cout << board;
        updateMoves();
    }

    void updateMoves(){
        // remember to update moves for each turn
        human.updateMoves();
        comp.updateMoves();
        board.updateCount();
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
