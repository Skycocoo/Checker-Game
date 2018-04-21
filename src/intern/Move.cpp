// Created by Yuxi Luo, April 2018
// For CS4613 Artificial Intelligence; Checker Game project

#include "../extern/Move.h"
#include "../extern/Board.h"

// standard output for Point
std::ostream& operator<<(std::ostream& os, const Point& p){
    os << "(" << p.x << ", " << p.y << ") ";
    return os;
}

// constructor
Point::Point(int x, int y): x(x), y(y){}

// convert to boolean if necessary
Point::operator bool() const {
    return (x != -1 && y != -1);
}

// update position
void Point::update(int x, int y){
    this->x = x;
    this->y = y;
}


// standard output for Move
// print out the available moves for current checker
std::ostream& operator<<(std::ostream& os, const Move& move){
    os << "\tChecker " << move.cur << " \n";

    if (move.isCapture()){
        os << "\t\tCapture move: ";
        for (size_t i = 0; i < move.capture.size(); i++) {
            if (move.capture[i]) os << move.capture[i] << " ";
        }
    } else if (move.isRegular()){
        os << "\t\tRegular move: ";
        for (size_t i = 0; i < move.regular.size(); i++) {
            if (move.regular[i]) os << move.regular[i] << " ";
        }
    }
    os << "\n";
    return os;
}

// constructor
// two out of four possible moves: capture / regular
Move::Move(int x, int y, const Board& board, int type): cur(x, y), board(&board), isHuman(type == HUSS), isCaptured(false){
    capture.push_back(Point(-1, -1));
    capture.push_back(Point(-1, -1));
    regular.push_back(Point(-1, -1));
    regular.push_back(Point(-1, -1));

    // update availability
    updateMove();
}

// convert to boolean if necessary
// indicate availability for this checker
Move::operator bool() const {
    return (!isCaptured && (isCapture() || isRegular()));
}

// check selected position
// return: bool: corresponding position or not
bool Move::select(int x, int y) const {
    return (x == cur.x && y == cur.y);
}

// check target position
// return: bool: corresponding target position or not
bool Move::checkMove(int targx, int targy) const {
    return ((regular[0].x == targx && regular[0].y == targy) ||
        (capture[0].x == targx && capture[0].y == targy)     ||
        (regular[1].x == targx && regular[1].y == targy)     ||
        (capture[1].x == targx && capture[1].y == targy));
}

// clear up this checker
void Move::clearMove(){
    for (size_t i = 0; i < capture.size(); i++) capture[i].update(-1, -1);
    for (size_t i = 0; i < regular.size(); i++) regular[i].update(-1, -1);
}

// update availability
// both capture move & regular move
void Move::updateMove(){
    // does not update move for captured checker
    if (isCaptured) return;

    // clear up the outdated move
    int x = cur.x, y = cur.y;
    clearMove();

    // setup possible positions
    int left1y = y - 1, right1y = y + 1, left2y = y - 2, right2y = y + 2;
    int new1x = x - 1, new2x = x - 2;
    // if the move is for enemy
    if (!isHuman) new1x = x + 1, new2x = x + 2;

    // if capture move is available
    if (new2x >= 0 && new2x < board->b.size()){
        // if this checker belongs to human
        if (isHuman){
            // if left capture is available
            if (left2y >= 0 && board->b[new1x][left1y] == COMP && board->b[new2x][left2y] == 0) capture[0].update(new2x, left2y);
            // if right capture is available
            if (right2y < board->b[new2x].size() && board->b[new1x][right1y] == COMP && board->b[new2x][right2y] == 0) capture[1].update(new2x, right2y);
        } else {
            // if this checker belongs to computer
            // if left capture is available
            if (left2y >= 0 && board->b[new1x][left1y] == HUSS && board->b[new2x][left2y] == 0) capture[0].update(new2x, left2y);
            // if right capture is available
            if (right2y < board->b[new2x].size() && board->b[new1x][right1y] == HUSS && board->b[new2x][right2y] == 0) capture[1].update(new2x, right2y);
        }
        // only allow capture move
        if (isCapture()) return;
    }

    // if regular move is available
    if (new1x >= 0 && new1x < board->b.size()){
        // if left regular is available
        if (left1y >= 0 && board->b[new1x][left1y] == 0) regular[0].update(new1x, left1y);
        // if right regular is available
        if (right1y < board->b[new1x].size() && board->b[new1x][right1y] == 0) regular[1].update(new1x, right1y);
    }
}

// update position
void Move::updatePos(int x, int y){
    cur.x = x;
    cur.y = y;
}

// update board
void Move::updateBoard(const Board* board){
    this->board = board;
}

// if the checker has capture move
// return: bool
bool Move::isCapture() const {
    return (capture[0] || capture[1]);
}

// if the checker has regular move
// return: bool
bool Move::isRegular() const {
    return (regular[0] || regular[1]);
}

// get the number of available moves
// return: int
int Move::getMoves() const {
    int count = 0;
    if (isCapture()){
        if (capture[0]) ++count;
        if (capture[1]) ++count;
        return count;
    } else {
        if (regular[0]) ++count;
        if (regular[1]) ++count;
        return count;
    }
    return count;
}

// if this checker is captured
bool Move::getCap() const {
    return isCaptured;
}

// capture this checker
void Move::captured(){
    isCaptured = true;
    clearMove();
}

// uncapture this checker
void Move::uncaptured(){
    isCaptured = false;
}
