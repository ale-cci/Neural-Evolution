#include <iostream>
#include "debug.h"

void warning(std::string id, std::string msg) {
    std::cout << id << " " << msg << std::endl;
    return ;
}

int rand(int LOWER, int UPPER) {
    return (rand() % (UPPER - LOWER + 1 )) + LOWER;
}

double rand(double LOWER, double UPPER) {
    double _ret = (double) rand() / RAND_MAX;
    return _ret * (UPPER - LOWER) + LOWER;
}
