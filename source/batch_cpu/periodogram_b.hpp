#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm> //for sorting
#include <tuple>
#include <cmath>

#include "GLS/GLS_b.hpp"
#include <boost/spirit/include/qi.hpp>
#include "../extras/readout.hpp"

using namespace std;


/*struct output_data {
	float power, amplitude, frequency;
	double sum_of_powers;
};*/


std::tuple<float, float, float> periodogram(float* frequencies_array, float step_size, int number_of_steps, std::filesystem::path in_file){

photometry data = read_dat(in_file);

//unsigned int length_of_data = size(data.x); float t[length_of_data]; float y[length_of_data]; float e_y[length_of_data]; //declares variables used by GLS_periodogram
unsigned int length_of_data = size(data.x); float *t; float *y; float *e_y; //declares variables used by GLS_periodogram

output_data best_frequency;

best_frequency = gls_b(data.x.data(), data.y.data(), data.dy.data(), length_of_data, number_of_steps, step_size, frequencies_array); //declares and fills a powers array //

float powers_average = best_frequency.sum_of_powers / number_of_steps; //calculates average power for the input data


std::tuple<float, float, float> output_tuple = make_tuple(best_frequency.frequency, best_frequency.amplitude, best_frequency.power / (powers_average * 2. * log2(length_of_data) ));

return output_tuple;
}
