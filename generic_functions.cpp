#include "generic_functions.h"
#include <cstdlib>
#include <cmath>

int rand(int LOWER, int UPPER) {
    return (rand() % (UPPER - LOWER + 1 )) + LOWER;
}

double rand(double LOWER, double UPPER) {
    double _ret = (double) rand() / RAND_MAX;
    return _ret * (UPPER - LOWER) + LOWER;
}


double special_one(double prop_value, double input) {
    double X = input - prop_value;
    return 1 / (1 + exp(A * X));
}
