#pragma once
#include <string>

#define PI 3.14159265

const static double A = -6;

typedef double _PRECISION;
typedef float STRENGHT_PRECISION;

void printagent(struct AGENT * _agent);
void warning(std::string id, std::string msg);
int rand(int LOWER, int UPPER);
double rand(double LOWER, double UPPER);

double special_one(double prop_value, double x);
