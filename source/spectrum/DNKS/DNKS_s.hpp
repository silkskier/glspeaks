#include <cmath>
#include <cstdlib>
#include <iostream>
#include <algorithm>

#include "../../extras/vertex.hpp"

   /*
    * t : time array
    * y : data array
    * n : length of data
   * nk : number of frequencies
   * fstep : step size for frequency
    * f : frequency array
    * p : powers (output) array
    */

void dnks_s(double* t,double* y, unsigned int n, unsigned int nk, double* f, double* p) {
	double average = 0; //y average value
    double y_norm = 0; //y normalization constant
	measurement *input = (measurement *) malloc(n * sizeof(measurement));
	const double n_inv = 1. / double(n);
	const double n_float = double(n);

	for (unsigned int i=0; i<n; ++i) {
		input[i].x = t[i];
		input[i].y = y[i];
		average += y[i];
   }

	average *= n_inv;

	for (unsigned int i=0; i<n; ++i) {input[i].y -= average;}
	for (unsigned int i=0; i<n; ++i) {y_norm += fabs(input[i].y);}
	y_norm = 2. / y_norm; for (unsigned int i=0; i<n; ++i) {input[i].y *= y_norm;}


	//#pragma omp parallel for
	for (unsigned int i=0; i<nk; ++i) {	//main loop

		double x_uuf = 0, y_uuf = 0; //ununiformity of x and y values
		double y_cumsum_min = 0, y_cumsum_max = 0, x_cumsum_min = 0, x_cumsum_max = 0; //cumsum values used for calculation of ununiformities
		double x_diff = 0;
		double y_cumsum = 0;

		measurement* input_temp = (measurement*)malloc(n * sizeof(measurement)); std::copy(input, input + n, input_temp);
		for (unsigned int j=0; j<n; ++j) {input_temp[j].x *= f[i]; input_temp[j].x -= double(int(input_temp[j].x));}// fmod is 6 times slower than that

		std::stable_sort(input_temp, input_temp + n, [](const measurement& a, const measurement& b) {return a.x < b.x;}); //sort the input array by phase, stable_sort is >4x faster, than std::sort

		for (unsigned int j=0; j<n; ++j) {
		y_cumsum += input_temp[j].y;
		if (y_cumsum < y_cumsum_min) {x_cumsum_min = y_cumsum;}
		else if (y_cumsum > y_cumsum_max) {y_cumsum_max = y_cumsum;}
		}

		p[i] += y_cumsum_max - y_cumsum_min;



		for (unsigned int j=0; j<n; ++j) {
			x_diff += input_temp[j].x - (double(j) * n_inv);
			if (x_diff < x_cumsum_min) {x_cumsum_min = x_diff;}
			else if (x_diff > x_cumsum_max) {x_cumsum_max = x_diff;}
		}

		p[i] -= 2 * (x_cumsum_max - x_cumsum_min) * n_inv; //works - ununiformity index of x axis (related to probability of aliasing)

		//for (unsigned int j=0; j<n; ++j){std::cout << input_temp[j].x <<std::endl;} // works



	free(input_temp);} //main loop



free(input);
}
