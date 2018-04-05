#include "../extern/HumanMoves.h"
#include "../extern/Board.h"
#include "../extern/Move.h"


// provide all avaliable moves
std::ostream& operator<<(std::ostream& os, const HumanMoves& h){
    os << "You have " << h.avaMoves() << " avaliable moves\n";
    for (size_t i = 0; i < h.moves.size(); i++){
        if (h.moves[i]) os << h.moves[i];
    }
    os << "\n";
    return os;
}

HumanMoves::HumanMoves(const Board& board): type(HUSS), cur(-1), board(&board){
    for (size_t i = 0; i < this->board->b.size(); i++){
        for (size_t j = 0; j < this->board->b[i].size(); j++){
            if (this->board->b[i][j] == type){
                moves.push_back(Move(i, j, board, type));
            }
        }
    }
    updateMoves();
}

void HumanMoves::updateMoves(){
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


bool HumanMoves::select(int x, int y) {
    cur = -1;
    for (size_t i = 0; i < moves.size(); i++){
        if (moves[i].select(x, y) && moves[i]){
            cur = i;
            std::cout << "You selected " << moves[cur];
            return true;
        }
    }
    // cout << "Not a legal selection" << endl;
    return false;
}

bool HumanMoves::checkMove(int targx, int targy) {
    bool result = moves[cur].checkMove(targx, targy);
    if (result) moves[cur].updatePos(targx, targy);
    return result;
}



bool HumanMoves::avaCapture() const {
    for (size_t i = 0; i < moves.size(); i++){
        if (moves[i].isCapture()) return true;
    }
    return false;
}

int HumanMoves::avaMoves() const {
    int count = 0;
    for (size_t i = 0; i < moves.size(); i++){
        if (moves[i]) ++count;
    }
    return count;
}
