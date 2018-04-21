// Created by Yuxi Luo, April 2018
// For CS4613 Artificial Intelligence; Checker Game project

#ifndef RESULT_H
#define RESULT_H

#include <iostream>

class Result{
public:
    int x, y, targX, targY;

    Result(int x, int y, int targX, int targY);
    void update(int x, int y, int targX, int targY);
    void update(const Result& r);
};

std::ostream& operator<<(std::ostream& os, const Result& r);

#endif
