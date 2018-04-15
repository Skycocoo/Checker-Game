#ifndef SEARCH_H
#define SEARCH_H

#include <chrono>
#include "AvaMoves.h"
#include "Board.h"
#include "Result.h"

class Search{
public:
    AvaMoves human;
    AvaMoves comp;

    Search(const AvaMoves& human, const AvaMoves& comp, const Board& board);

    void updateBoard(const Board& board);

    void updateMoves();
    Result search(const Board& board);

    Result iterativeDeep(int maxDepth = 3);
    // return the action or the estimated value?
    float alphaBeta(Result& fmove, int depth);

    // maxVal: for COMP player
    float maxVal(float alpha, float beta, Result& fmove, int depth);
    // minVal: for HUSS player
    float minVal(float alpha, float beta, Result& fmove, int depth);


    // move from (x, y) to (targx, targy)
    // should also take care of the checkers
    void move(int x, int y, int targx, int targy, int type);

    // reset the move from (x, y) to (targx, targy) back to original
    void reset(int x, int y, int targx, int targy, int type);


    // heuristics to estimate the expected utility
    float eval(int type = HUSS) const ;
    // utility value for terminal state: range [-6, 6] = numC - numH
    float utility() const;
    bool terminalState() const;

private:
    Board board;
    std::chrono::system_clock::time_point start;
};

#endif