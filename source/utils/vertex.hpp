#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <iostream>

struct output_data {
    float power = 0.0f;
    float amplitude = 0.0f;
    double sum_of_powers = 0.0;
    double frequency = 0.0;
};

struct measurement {
    float x;
    float y;
};

std::pair<double, double> vertex(double x1, double y1, double x2, double y2, double x3, double y3){
    double denom = (x1 - x2) * (x1 - x3) * (x2 - x3);
    double A     = (x3 * (y2 - y1) + x2 * (y1 - y3) + x1 * (y3 - y2)) / denom;
    double B     = (x3*x3 * (y1 - y2) + x2*x2 * (y3 - y1) + x1*x1 * (y2 - y3)) / denom;
    double C     = (x2 * x3 * (x2 - x3) * y1 + x3 * x1 * (x3 - x1) * y2 + x1 * x2 * (x1 - x2) * y3) / denom;

    double xv = -B / (2*A);
    double yv = C - B*B / (4*A);

    return std::make_pair(xv, yv);
}

#endif
