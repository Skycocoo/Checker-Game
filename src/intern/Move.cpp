#include "../extern/Move.h"

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

// both capture move & regular move?
void Move::updateMove(){
    int x = cur.x, y = cur.y;

    if (isHuman){
        int up1x = x - 1, up2x = x - 2,
            left1y = y - 1, right1y = y + 1, left2y = y - 2, right2y = y + 2;

        if (up2x >= 0){
            if (left2y >= 0 && board->b[up1x][left1y] == COMP && board->b[up2x][left2y] == 0) left2.update(up2x, left2y);
            if (right2y < board->b[up2x].size() && board->b[up1x][right1y] == COMP && board->b[up2x][right2y] == 0) right2.update(up2x, right2y);
        }

        if (up1x >= 0){
            if (left1y >= 0 && board->b[up1x][left1y] == 0) left1.update(up1x, left1y);
            if (right1y < board->b[up1x].size() && board->b[up1x][right1y] == 0) right1.update(up1x, right1y);
        }

    } else {
        // this is moves for computer
    }

}

std::ostream& operator<<(std::ostream& os, const Move& move){
    os << "Checker (" << move.cur.x << ", " << move.cur.y << ")\n";
    if (move.isCapture()){
        os << "\tCapture move:";
        if (move.left2) os << " (" << move.left2.x << ", " << move.left2.y << ")";
        if (move.right2) os << " (" << move.right2.x << ", " << move.right2.y << ")";
        os << "\n";
    } else if (move.isRegular()){
        os << "\tRegular move:";
        if (move.left1) os << " (" << move.left1.x << ", " << move.left1.y << ")";
        if (move.right1) os << " (" << move.right1.x << ", " << move.right1.y << ")";
        os << "\n";
    }
    return os;
}
