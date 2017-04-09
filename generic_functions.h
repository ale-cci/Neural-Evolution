#pragma once
#include <string>


const static double A = -6;
extern std::string texture_path;

typedef double _PRECISION;
typedef float STRENGHT_PRECISION;

void printagent(struct AGENT * _agent);
void warning(std::string id, std::string msg);
int rand(int LOWER, int UPPER);
double rand(double LOWER, double UPPER);

double special_one(double input);
