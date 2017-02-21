#include <iostream>
#include "debug.h"

void warning(std::string id, std::string msg) {
    std::cout << id << " " << msg << std::endl;
    return ;
}

int rand(int LOWER, int UPPER) {
    return (rand() % (UPPER - LOWER + 1 )) + LOWER;
}
