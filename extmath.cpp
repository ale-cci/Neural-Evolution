#include "extmath.h"
#include <cmath>
#include <cassert>

double delta(double A, double B, double C) {
    double solution = B*B - 4*A*C;
    if (solution < 0)
        return -1;
    return sqrt(solution);
}

double*  deg2equation(double A, double B, double C, double * solutions, uint8_t* solution_count) {
    double D = delta(A, B, C);
    if (D < 0) {
        *solution_count = 0;
        return nullptr;
    }
    *solution_count = 2;
    solutions[0] = (-B + D) / (2 * A);
    solutions[1] = (-B - D) / (2 * A);
    return solutions;
}

void line_circle_collision(double m_line, double q_line, double x_center, double y_center, double radius, COORD* output, uint8_t* solution_count) {
    double solutions[2];
    double  A = pow(m_line, 2) + 1,
            B = 2*(m_line * (q_line - y_center) - x_center),
            C = pow(q_line - y_center, 2) + pow(x_center, 2) - pow(radius, 2);
    deg2equation( A, B, C, solutions, solution_count);
    for (int i = 0; i < *solution_count; ++i)
        output[i] = {solutions[i], m_line * solutions[i] + q_line};
}

double distance(COORD* A, COORD* B) {
    return sqrt(pow((A->X - B->X), 2) + pow((A->Y - B->Y), 2));
}
double distance(COORD A, COORD B) {
    return sqrt(pow((A.X - B.X), 2) + pow(A.Y - B.Y, 2));
}

double mod360(double alpha) {
    return modX(alpha, 360.);
}

double mod2PI(double alpha) {
    return modX(alpha, 2 * PI);
}
double modX(double alpha, double X) {
    assert(X);
    int64_t delta = alpha / X;
    return alpha + (-delta + (alpha < 0)) * X;
}

double getangle(double SIN, double COS) {
    return modX(acos(COS) * ((SIN > 0) ? 1 : -1) , 2 * PI);
}

double sex_rad(const double alpha) {
    return alpha * PI / 180;
}
double rad_sex(const double alpha) {
    return alpha * 180 / PI;
}

double bound(const double value, const double lower, const double upper) {
    if (value <= lower)
        return lower;
    if (value >= upper)
        return upper;
    return value;
}
