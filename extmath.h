#pragma once
#include <inttypes.h>
#include <cmath>

#define PI 3.14159265

struct COORD {
    double X;
    double Y;
};

double delta(double A, double B, double C, uint8_t& solution_count);
double* deg2equation(double A, double B, double C, uint8_t* solution_count);
void line_circle_collision(double m_line, double q_line, double x_center, double y_center, double radius, COORD* output, uint8_t* solution_count);
double distance(COORD* A, COORD* B);
double distance(COORD A, COORD B);
double mod360(double alpha);
double mod2PI(double alpha);
double modX(double alpha, double X);
double getangle(double SIN, double COS);
double sex_rad(const double alpha);
double rad_sex(const double alpha);
double bound(const double value, const double lower, const double upper);


int rand(int lb, int ub);
double rand(double lb, double ub);

