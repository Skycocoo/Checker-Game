#include "../extern/Result.h"


Result::Result(int x, int y, int targX, int targY): x(x), y(y), targX(targX), targY(targY){}

void Result::update(int x, int y, int targX, int targY){
    this->x = x;
    this->y = y;
    this->targX = targX;
    this->targY = targY;
}
void Result::update(const Result& r){
    this->x = r.x;
    this->y = r.y;
    this->targX = r.targX;
    this->targY = r.targY;
}

std::ostream& operator<<(std::ostream& os, const Result& r){
    os << "\tFrom (" << r.x << ", " << r.y << ") to (" << r.targX << ", " << r.targY << ")\n";
    return os;
}
