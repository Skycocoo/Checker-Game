// Created by Yuxi Luo, April 2018
// For CS4613 Artificial Intelligence; Checker Game project

#include "../extern/AvaMoves.h"
#include "../extern/Board.h"
#include "../extern/Move.h"


// standard output for AvaMoves
// print out the available moves for player
// return: std::ostream&
std::ostream& operator<<(std::ostream& os, const AvaMoves& a){
    if (a.type == HUSS) os << "Human: ";
    else os << "Computer: ";

    os << a.avaMoves() << " available moves\n";
    for (size_t i = 0; i < a.moves.size(); i++){
        if (a.moves[i]) os << a.moves[i];
    }
    os << "\n";
    return os;
}

// constructor
// initialize moves's position based on the type (human / computer)
AvaMoves::AvaMoves(const Board& board, int type): type(type), cur(-1), board(&board){
    for (size_t i = 0; i < this->board->b.size(); i++){
        for (size_t j = 0; j < this->board->b[i].size(); j++){
            if (this->board->b[i][j] == type){
                moves.push_back(Move(i, j, board, type));
            }
        }
    }
    // update each move for availability
    updateMoves();
}

// update checker board for human / computer
// return: void
void AvaMoves::updateBoard(const Board& board){
    this->board = &board;
    // update board for each moves
    for (size_t i = 0; i < moves.size(); i++){
        moves[i].updateBoard(this->board);
    }
}

// update each move for availability
// return: void
void AvaMoves::updateMoves(){
    // all of the moves needed to update
    for (size_t i = 0; i < moves.size(); i++){
        moves[i].updateMove();
    }

    // only allow capture move for current player
    if (avaCapture()){
        for (size_t i = 0; i < moves.size(); i++){
            if (!moves[i].isCapture()) moves[i].clearMove();
        }
    }
}

// select checker to be moved
// return: bool: successfully select the checker or not
bool AvaMoves::select(int x, int y, bool output) {
    // memorize currect selection
    cur = -1;
    for (size_t i = 0; i < moves.size(); i++){
        // if the selected checker is available
        if (moves[i] && moves[i].select(x, y)){
            cur = i;
            if (output) std::cout << "You selected " << moves[cur];
            return true;
        }
    }
    return false;
}

// check the target position for selected checker
// return: bool: legal target position or not
bool AvaMoves::checkMove(int targX, int targY) {
    // if the target position is legal
    if (moves[cur].checkMove(targX, targY)) {
        // move the selected checker
        moves[cur].updatePos(targX, targY);
        return true;
    }
    return false;
}

// the corresponding checker captured by enemy
// return: int: the index of captured checker
int AvaMoves::captured(int x, int y){
    for (size_t i = 0; i < moves.size(); i++){
        // if the position matched with the captured checker
        if (!moves[i].getCap() && moves[i].select(x, y)){
            // mark the checker as captured
            moves[i].captured();
            return i;
        }
    }
    return -1;
}

// ------------ helper function for Search ------------ //
// because Search performs entirely on index
// manipulate correct indexing is necessary

// move the checker by index
// return: bool: sucess move or not
bool AvaMoves::superMove(int index, int targX, int targY){
    if (moves[index].checkMove(targX, targY)) {
        moves[index].updatePos(targX, targY);
        return true;
    }
    return false;
}

// reset the checker by index
void AvaMoves::reset(int index, int x, int y){
    moves[index].updatePos(x, y);
}

// reset the captured checker by index
void AvaMoves::resetCaptured(int index){
    moves[index].uncaptured();
}

// ------------ end of helper functions ------------ //


// there is available capture move
// return: bool: available capture move or not
bool AvaMoves::avaCapture() const {
    for (size_t i = 0; i < moves.size(); i++){
        if (!moves[i].getCap() && moves[i].isCapture()) return true;
    }
    return false;
}

// number of available moves for current player
int AvaMoves::avaMoves() const {
    int count = 0;
    for (size_t i = 0; i < moves.size(); i++){
        // if the checker is available: update count for available moves
        if (moves[i]) count += moves[i].getMoves();
    }
    return count;
}

// for heuristics
// check the distance from the current player to the other end of the board
// return: int: distance of the current player
int AvaMoves::distance() const {
    int count = 0;
    for (size_t i = 0; i < moves.size(); i++){
        // ignore captured checkers
        if (moves[i].getCap()) continue;

        if (type == HUSS) count += moves[i].cur.x;
        else count += (board->b.size() - moves[i].cur.x - 1);
    }
    return count;
}
