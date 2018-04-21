#include "../extern/AvaMoves.h"
#include "../extern/Board.h"
#include "../extern/Move.h"


// provide all available moves
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

AvaMoves::AvaMoves(const Board& board, int type): type(type), cur(-1), board(&board){
    for (size_t i = 0; i < this->board->b.size(); i++){
        for (size_t j = 0; j < this->board->b[i].size(); j++){
            if (this->board->b[i][j] == type){
                moves.push_back(Move(i, j, board, type));
            }
        }
    }
    updateMoves();
}

void AvaMoves::updateMoves(){
    // all of the moves needed to update
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

void AvaMoves::updateBoard(const Board& board){
    this->board = &board;
    for (size_t i = 0; i < moves.size(); i++){
        moves[i].updateBoard(this->board);
    }
}


bool AvaMoves::select(int x, int y, bool output) {
    cur = -1;
    for (size_t i = 0; i < moves.size(); i++){
        if (moves[i] && moves[i].select(x, y)){
            cur = i;
            if (output) std::cout << "You selected " << moves[cur];
            return true;
        }
    }
    return false;
}

bool AvaMoves::checkMove(int targX, int targY) {
    if (moves[cur].checkMove(targX, targY)) {
        moves[cur].updatePos(targX, targY);
        return true;
    }
    return false;
}

int AvaMoves::captured(int x, int y){
    for (size_t i = 0; i < moves.size(); i++){
        if (!moves[i].getCap() && moves[i].select(x, y)){
            moves[i].captured();
            return i;
        }
    }
    return -1;
}

// use correct indexing for search
bool AvaMoves::superMove(int index, int targX, int targY){
    if (moves[index].checkMove(targX, targY)) {
        moves[index].updatePos(targX, targY);
        return true;
    }
    return false;
}

void AvaMoves::reset(int index, int x, int y){
    moves[index].updatePos(x, y);
}

void AvaMoves::resetCaptured(int index){
    moves[index].uncaptured();
}


bool AvaMoves::avaCapture() const {
    for (size_t i = 0; i < moves.size(); i++){
        if (!moves[i].getCap() && moves[i].isCapture()) return true;
    }
    return false;
}

int AvaMoves::avaMoves() const {
    int count = 0;
    for (size_t i = 0; i < moves.size(); i++){
        if (moves[i]) count += moves[i].getMoves();
    }
    return count;
}

int AvaMoves::distance() const {
    int count = 0;
    for (size_t i = 0; i < moves.size(); i++){
        if (moves[i].getCap()) continue;

        if (type == HUSS) count += moves[i].cur.x;
        else count += (board->b.size() - moves[i].cur.x - 1);
    }
    return count;
}
