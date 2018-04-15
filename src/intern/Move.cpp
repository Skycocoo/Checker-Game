#include "../extern/Move.h"
#include "../extern/Board.h"

Point::Point(int x, int y): x(x), y(y){}

void Point::update(int x, int y){
    this->x = x;
    this->y = y;
}

Point::operator bool() const {
    return (x != -1 && y != -1);
}


Move::Move(int x, int y, const Board& board, int type): board(&board), isHuman(type == HUSS), cur(x, y){
    capture.push_back(Point(-1, -1));
    capture.push_back(Point(-1, -1));
    regular.push_back(Point(-1, -1));
    regular.push_back(Point(-1, -1));
    updateMove();
}

bool Move::isCapture() const {
    return (capture[0] || capture[1]);
}

bool Move::isRegular() const {
    return (regular[0] || regular[1]);
}


bool Move::select(int x, int y) const {
    return (x == cur.x && y == cur.y);
}

bool Move::checkMove(int targx, int targy) const {
    return ((regular[0].x == targx && regular[0].y == targy) ||
        (capture[0].x == targx && capture[0].y == targy)     ||
        (regular[1].x == targx && regular[1].y == targy)   ||
        (capture[1].x == targx && capture[1].y == targy));
}

void Move::updatePos(int x, int y){
    cur.x = x;
    cur.y = y;
}


// both capture move & regular move?
void Move::updateMove(){
    int x = cur.x, y = cur.y;
    clearMove();

    int left1y = y - 1, right1y = y + 1, left2y = y - 2, right2y = y + 2;
    int new1x = x - 1, new2x = x - 2;

    // if the move is for enemy
    if (!isHuman) new1x = x + 1, new2x = x + 2;

    if (new2x >= 0 && new2x < board->b.size()){
        if (isHuman){
            if (left2y >= 0 && board->b[new1x][left1y] == COMP && board->b[new2x][left2y] == 0) capture[0].update(new2x, left2y);
            if (right2y < board->b[new2x].size() && board->b[new1x][right1y] == COMP && board->b[new2x][right2y] == 0) capture[1].update(new2x, right2y);
        } else {
            if (left2y >= 0 && board->b[new1x][left1y] == HUSS && board->b[new2x][left2y] == 0) capture[0].update(new2x, left2y);
            if (right2y < board->b[new2x].size() && board->b[new1x][right1y] == HUSS && board->b[new2x][right2y] == 0) capture[1].update(new2x, right2y);
        }
        // only allow capture move for this checker
        if (isCapture()) return;
    }

    if (new1x >= 0 && new1x < board->b.size()){
        if (left1y >= 0 && board->b[new1x][left1y] == 0) regular[0].update(new1x, left1y);
        if (right1y < board->b[new1x].size() && board->b[new1x][right1y] == 0) regular[1].update(new1x, right1y);
    }
}

void Move::clearMove(){
    for (size_t i = 0; i < capture.size(); i++) capture[i].update(-1, -1);
    for (size_t i = 0; i < regular.size(); i++) regular[i].update(-1, -1);
}

void Move::updateBoard(const Board* board){
    this->board = board;
}


Move::operator bool() const{
    return (isCapture() || isRegular());
}


std::ostream& operator<<(std::ostream& os, const Move& move){
    os << "\tChecker (" << move.cur.x << ", " << move.cur.y << ")\n";

    if (move.isCapture()){
        os << "\t\tCapture move:";
        for (size_t i = 0; i < move.capture.size(); i++) {
            if (move.capture[i]) os << " (" << move.capture[i].x << ", " << move.capture[i].y << ")";
        }
        os << "\n";
        return os;
    }

    if (move.isRegular()){
        os << "\t\tRegular move:";
        for (size_t i = 0; i < move.regular.size(); i++) {
            if (move.regular[i]) os << " (" << move.regular[i].x << ", " << move.regular[i].y << ")";
        }
        os << "\n";
    }
    return os;
}
