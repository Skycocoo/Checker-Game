// Created by Yuxi Luo, April 2018
// For CS4613 Artificial Intelligence; Checker Game project

#include "../extern/Result.h"


// constructor
Result::Result(int x, int y, int targX, int targY): x(x), y(y), targX(targX), targY(targY){}

// update positions by integers
void Result::update(int x, int y, int targX, int targY){
    this->x = x;
    this->y = y;
    this->targX = targX;
    this->targY = targY;
}

// update positions by Result
void Result::update(const Result& r){
    this->x = r.x;
    this->y = r.y;
    this->targX = r.targX;
    this->targY = r.targY;
}

// standard output for Result
std::ostream& operator<<(std::ostream& os, const Result& r){
    os << "\tFrom (" << r.x << ", " << r.y << ") to (" << r.targX << ", " << r.targY << ")\n";
    return os;
}
