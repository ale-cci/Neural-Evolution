#include "generic_functions.h"
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <cstdio>
#include <fstream>
#include "extmath.h"
/*

double special_one(double input) {
    double ret = 1/(1+exp(A*input));
    return ret;
}
*/

double special_one(double input) {
    assert(!std::isnan(input));
    return sin(input*PI);
}


void firstfree(char* filename, const char* _string) {
    std::ifstream in;
    char tempstring[64];
    for (int64_t i=0; i < 128; ++i) {
        sprintf(tempstring, _string, i);
        in.open(tempstring);
        if (in.fail())
            break;
    }
    if (in.fail())
        strcpy(filename, tempstring);
    in.close();
}
