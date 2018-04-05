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


Move::Move(int x, int y, const Board& board, int type): cur(x, y), board(&board), isHuman(false){
    if (type == HUSS) isHuman = true;
    updateMove();
}

bool Move::isCapture() const {
    return (left2 || right2);
}

bool Move::isRegular() const {
    return (left1 || right1);
}


bool Move::select(int x, int y) const {
    return (x == cur.x && y == cur.y);
}

bool Move::checkMove(int targx, int targy) const {
    return ((left1.x == targx && left1.y == targy) ||
        (left2.x == targx && left2.y == targy)     ||
        (right1.x == targx && right1.y == targy)   ||
        (right2.x == targx && right2.y == targy));
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

    if (!isHuman) new1x = x + 1, new2x = x + 2;

    if (new2x >= 0 && new2x < board->b.size()){
        if (isHuman){
            if (left2y >= 0 && board->b[new1x][left1y] == COMP && board->b[new2x][left2y] == 0) left2.update(new2x, left2y);
            if (right2y < board->b[new2x].size() && board->b[new1x][right1y] == COMP && board->b[new2x][right2y] == 0) right2.update(new2x, right2y);
        } else {
            if (left2y >= 0 && board->b[new1x][left1y] == HUSS && board->b[new2x][left2y] == 0) left2.update(new2x, left2y);
            if (right2y < board->b[new2x].size() && board->b[new1x][right1y] == HUSS && board->b[new2x][right2y] == 0) right2.update(new2x, right2y);
        }
        // only allow capture move for this checker
        if (isCapture()) return;
    }

    if (new1x >= 0 && new1x < board->b.size()){
        if (left1y >= 0 && board->b[new1x][left1y] == 0) left1.update(new1x, left1y);
        if (right1y < board->b[new1x].size() && board->b[new1x][right1y] == 0) right1.update(new1x, right1y);
    }
}

void Move::clearMove(){
    left1.update(-1, -1);
    left2.update(-1, -1);
    right1.update(-1, -1);
    right2.update(-1, -1);
}

Move::operator bool() const{
    return (isCapture() || isRegular());
}


std::ostream& operator<<(std::ostream& os, const Move& move){
    os << "\tChecker (" << move.cur.x << ", " << move.cur.y << ")\n";

    if (move.isCapture()){
        os << "\t\tCapture move:";
        if (move.left2) os << " (" << move.left2.x << ", " << move.left2.y << ")";
        if (move.right2) os << " (" << move.right2.x << ", " << move.right2.y << ")";
        os << "\n";
        return os;
    }

    if (move.isRegular()){
        os << "\t\tRegular move:";
        if (move.left1) os << " (" << move.left1.x << ", " << move.left1.y << ")";
        if (move.right1) os << " (" << move.right1.x << ", " << move.right1.y << ")";
        os << "\n";
    }
    return os;
}
