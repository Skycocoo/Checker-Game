// Created by Yuxi Luo, April 2018
// For CS4613 Artificial Intelligence; Checker Game project

#ifndef SEARCH_H
#define SEARCH_H

#include <chrono>
#include "AvaMoves.h"
#include "Board.h"
#include "Result.h"

#include "Text.h"


class Checker;

// class Search:
// an abstraction of search algorithm for Checker Game
// utilize Alpha-beta Pruning algorithm under Min-max search
class Search{
public:
    AvaMoves human;
    AvaMoves comp;

    // constructor
    Search(const AvaMoves& human, const AvaMoves& comp, const Board& board, Checker* check);


    void renderSearch(float util, int min, int max, int node);



    // update availability for both human and computer
    void updateMoves();
    // get the only move for computer
    Result getMove() const;

    // start the search for this turn
    Result search(const Board& board);
    // use iterative deepening search to set timed cutoff
    Result iterativeDeep(int maxDepth = 80);

    // alpha-beta search based on cutoff depth
    float alphaBeta(Result& fmove, int depth, int& numMax, int& numMin, int& numNode);

    // maxVal: for COMP player
    float maxVal(float alpha, float beta, Result& fmove, int curDepth, int depth, int& numMax, int& numMin, int& numNode);
    // minVal: for HUSS player
    float minVal(float alpha, float beta, Result& fmove, int curDepth, int depth, int& numMax, int& numMin, int& numNode);

    // update moves from the actual game
    void update(int x, int y, int targX, int targY, int type);

    // move from (x, y) to (targx, targy)
    int move(int index, int x, int y, int targX, int targY, int type);
    // reset the move from (x, y) to (targx, targy) back to original
    void reset(int index, int indexCap, int x, int y, int targX, int targY, int type);


    // heuristics to estimate the expected utility
    float eval() const ;
    // utility value for terminal state: range [-6, 6] = numC - numH
    float utility() const;

    // check terminal state for the game
    bool terminalState() const;

private:
    Board board;
    std::chrono::system_clock::time_point start;

    Checker* check;
    Text text;
};

#endif
