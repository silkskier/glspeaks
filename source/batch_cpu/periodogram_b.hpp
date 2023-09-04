#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm> //for sorting
#include <tuple>
#include <cmath>

#include "GLS/GLS_b.hpp"
#include "../extras/grid.hpp"
#include "../extras/readout.hpp"

using namespace std;


/*struct output_data {
	float power, amplitude, frequency;
	double sum_of_powers;
};*/


std::tuple<float, float, float> periodogram(const grid &grid, std::filesystem::path in_file){

	star data; data.read(in_file);

	output_data best_frequency;

	best_frequency = gls_b(data.x.data(), data.y.data(), data.dy.data(), data.x.size(), grid.freq.size(), grid.fstep, grid.freq.data()); //declares and fills a powers array //

	float powers_average = best_frequency.sum_of_powers / double(grid.freq.size()); //calculates average power for the input data

	std::tuple<double, float, float> output_tuple = make_tuple(best_frequency.frequency, best_frequency.amplitude, best_frequency.power / (powers_average * 2. * log2(data.x.size()) ));

return output_tuple;
}
