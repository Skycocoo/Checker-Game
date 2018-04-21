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

    Result getMove() const;

    Result search(const Board& board);
    Result iterativeDeep(int maxDepth = 80);

    // return the action or the estimated value?
    float alphaBeta(Result& fmove, int depth, int& numMax, int& numMin, int& numNode);

    // maxVal: for COMP player
    float maxVal(float alpha, float beta, Result& fmove, int curDepth, int depth, int& numMax, int& numMin, int& numNode);
    // minVal: for HUSS player
    float minVal(float alpha, float beta, Result& fmove, int curDepth, int depth, int& numMax, int& numMin, int& numNode);

    void update(int x, int y, int targX, int targY, int type);

    // move from (x, y) to (targx, targy)
    // should also take care of the checkers
    int move(int index, int x, int y, int targX, int targY, int type);

    // reset the move from (x, y) to (targx, targy) back to original
    void reset(int index, int indexCap, int x, int y, int targX, int targY, int type);


    // heuristics to estimate the expected utility
    float eval() const ;
    // utility value for terminal state: range [-6, 6] = numC - numH
    float utility() const;
    bool terminalState() const;

private:
    Board board;
    std::chrono::system_clock::time_point start;
};

#endif
