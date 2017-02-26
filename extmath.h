#pragma once
#include <inttypes.h>

struct COORD {
    double X;
    double Y;
};

double delta(double A, double B, double C, uint8_t& solution_count);
double* deg2equation(double A, double B, double C, uint8_t* solution_count);
void line_circle_collision(double m_line, double q_line, double x_center, double y_center, double radius, COORD* output, uint8_t* solution_count);
double distance(COORD* A, COORD* B);
double mod360(double alpha);
double modX(double alpha, double X);
