#include <iostream>
#include "debug.h"

void warning(std::string id, std::string msg) {
    std::cout << id << " " << msg << std::endl;
    return ;
}
