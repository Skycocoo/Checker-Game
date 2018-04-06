#include "../extern/AvaMoves.h"
#include "../extern/Board.h"
#include "../extern/Move.h"


// provide all avaliable moves
std::ostream& operator<<(std::ostream& os, const AvaMoves& a){
    os << "You have " << a.avaMoves() << " avaliable moves\n";
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

void AvaMoves::updateBoard(const Board& board){
    this->board = &board;
}

void AvaMoves::updateMoves(){
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


bool AvaMoves::select(int x, int y) {
    cur = -1;
    for (size_t i = 0; i < moves.size(); i++){
        if (moves[i].select(x, y) && moves[i]){
            cur = i;
            std::cout << "You selected " << moves[cur];
            return true;
        }
    }
    return false;
}

bool AvaMoves::checkMove(int targx, int targy) {
    bool result = moves[cur].checkMove(targx, targy);
    if (result) moves[cur].updatePos(targx, targy);
    return result;
}


bool AvaMoves::avaCapture() const {
    for (size_t i = 0; i < moves.size(); i++){
        if (moves[i].isCapture()) return true;
    }
    return false;
}

int AvaMoves::avaMoves() const {
    int count = 0;
    for (size_t i = 0; i < moves.size(); i++){
        if (moves[i]) ++count;
    }
    return count;
}

int AvaMoves::proximity() const {
    int count = 0;
    for (size_t i = 0; i < moves.size(); i++){
        if (type == HUSS) count += moves[i].cur.x - 1;
        else count += (6 - moves[i].cur.x + 1);
    }
    return count;
}
