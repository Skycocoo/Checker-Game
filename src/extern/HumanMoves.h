#ifndef HUMANMOVES_H
#define HUMANMOVES_H

#include <iostream>
#include <vector>

class Board;
class Move;

class HumanMoves{
    // provide all avaliable moves
    friend std::ostream& operator<<(std::ostream& os, const HumanMoves& h);
public:
    HumanMoves(const Board& board);

    void updateMoves();

    bool select(int x, int y);
    bool checkMove(int targx, int targy) const;

    bool avaCapture() const;
    int avaMoves() const;
private:
    int type;
    int cur;

    const Board* board;
    std::vector<Move> moves;
};

#endif
