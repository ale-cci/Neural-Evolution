#pragma once
#include <string>
#define PI 3.14159265

typedef float _PRECISION;
typedef float STRENGHT_PRECISION;

void printagent(struct AGENT * _agent);
void warning(std::string id, std::string msg);
int rand(int LOWER, int UPPER);
double rand(double LOWER, double UPPER);
