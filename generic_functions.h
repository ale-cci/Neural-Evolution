#pragma once
#include <string>


const static double A = -6;
extern std::string texture_path;

typedef double _PRECISION;

void printagent(struct AGENT * _agent);
void warning(std::string id, std::string msg, ...);

double special_one(double input);
void  firstfree(char* str, const char* message);
