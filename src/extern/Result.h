// Created by Yuxi Luo, April 2018
// For CS4613 Artificial Intelligence; Checker Game project

#ifndef RESULT_H
#define RESULT_H

#include <iostream>

// class Result:
// an abstraction of the result from Search
class Result{
public:
    int x, y, targX, targY;

    // constructor
    Result(int x, int y, int targX, int targY);
    // update positions by integers
    void update(int x, int y, int targX, int targY);
    // update positions by Result
    void update(const Result& r);
};

// standard output for Result
std::ostream& operator<<(std::ostream& os, const Result& r);

#endif
