#include <cmath>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <math.h>

#include "../../extras/vertex.hpp"
#include "../../extras/pdqsort.h"


   /*
    * t : time array
    * y : data array
    * n : length of data
   * nk : number of frequencies
   * fstep : step size for frequency
    * f : frequency array
    * p : powers (output) array
    */

output_data dnks_b(float* t,float* y, unsigned int n, unsigned int nk, float* f) {

	output_data best_frequency; best_frequency.power = 0.; best_frequency.sum_of_powers = 0.;

	float average = 0; //y average value
    float y_norm = 0; //y normalization constant
	measurement *input = (measurement *) malloc(n * sizeof(measurement));
	const float n_inv = 1. / float(n);
	const float n_float = float(n);
	const float crit = 1 / sqrt(n);

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

		float p = 0; //slot for power

		float x_uuf = 0, y_uuf = 0; //ununiformity of x and y values
		float y_cumsum_min = 0, y_cumsum_max = 0, x_cumsum_min = 0, x_cumsum_max = 0; //cumsum values used for calculation of ununiformities
		float x_diff = 0;
		float y_cumsum = 0;

		measurement* input_temp = (measurement*)malloc(n * sizeof(measurement)); std::copy(input, input + n, input_temp);
		for (unsigned int j=0; j<n; ++j) {input_temp[j].x *= f[i]; input_temp[j].x -= float(int(input_temp[j].x));}// fmod is 6 times slower than that

		pdqsort_branchless(input_temp, input_temp + n, [](const measurement& a, const measurement& b) {
            		return a.x < b.x;}); //sort the input array by phase, pdqsort_branchless is ~2x faster, than std::sort

		for (unsigned int j=0; j<n; ++j) {
		y_cumsum += input_temp[j].y;
		if (y_cumsum < y_cumsum_min) {x_cumsum_min = y_cumsum;}
		else if (y_cumsum > y_cumsum_max) {y_cumsum_max = y_cumsum;}
		}

		p += y_cumsum_max - y_cumsum_min;



		for (unsigned int j=0; j<n; ++j) {
			x_diff += input_temp[j].x - (float(j) * n_inv);
			if (x_diff < x_cumsum_min) {x_cumsum_min = x_diff;}
			else if (x_diff > x_cumsum_max) {x_cumsum_max = x_diff;}
		}

		float x_ununiform = 2 * (x_cumsum_max - x_cumsum_min) * n_inv;
		p -= x_ununiform; //works - ununiformity index of x axis (related to probability of aliasing)
		//if ((p < 0) || (x_ununiform > crit)) { p = 0.; }

		if (p > float(best_frequency.power)) {best_frequency.power = p; best_frequency.frequency = f[i];}

		//for (unsigned int j=0; j<n; ++j){std::cout << input_temp[j].x <<std::endl;} // works



	free(input_temp);} //main loop

	best_frequency.amplitude = 1;


return best_frequency;}
