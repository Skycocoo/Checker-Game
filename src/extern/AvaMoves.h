#ifndef AVAMOVES_H
#define AVAMOVES_H

#include <iostream>
#include <vector>

class Board;
class Move;

class AvaMoves{
    // provide all avaliable moves
    friend std::ostream& operator<<(std::ostream& os, const AvaMoves& h);
public:
    AvaMoves(const Board& board, int type);

    void updateMoves();

    bool select(int x, int y);
    bool checkMove(int targx, int targy);

    bool avaCapture() const;
    int avaMoves() const;
private:
    int type;
    int cur;

    const Board* board;
    std::vector<Move> moves;
};

#endif
