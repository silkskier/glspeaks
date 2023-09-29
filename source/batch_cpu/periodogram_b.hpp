#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm> //for sorting
#include <tuple>
#include <cmath>

#include "GLS/GLS.hpp"
#include "GLS/NFFTLS.hpp" //WIP
#include "CNU/BCNU.hpp"
#include "CE/CE.hpp"

#include "../utils/grid.hpp"
#include "../utils/readout.hpp"

std::tuple<float, float, float> periodogram(Grid &grid, std::filesystem::path in_file){

	star data; data.read(in_file);
	output_data best_frequency;

	best_frequency = gls_b(data, grid); //recursive GLS
	//best_frequency = bncu_b(data, grid); //binned conditional nonuniformity, requires fixing weighting function
	//best_frequency = ce_b(data, grid);
	float powers_average = best_frequency.sum_of_powers / double(grid.freq.size()); //calculates average power for the input data

	std::tuple<double, float, float> output_tuple = std::make_tuple(best_frequency.frequency, best_frequency.amplitude, best_frequency.power / (powers_average * 2. * log2(data.x.size())));
return output_tuple;
}


std::tuple<float, float, float> periodogram(Grid &grid, std::filesystem::path in_file, finufft_opts* opts){
	//despite being polymorphic doesn't slow the execution down noticeably due to the size of periodogram functions

	star data; data.read(in_file);
	output_data best_frequency;
	float powers_average = best_frequency.sum_of_powers / double(grid.freq.size()); //calculates average power for the input data

	best_frequency = nfftls_b(data, grid, opts); //FFT-based GLS

	std::tuple<double, float, float> output_tuple = std::make_tuple(best_frequency.frequency, best_frequency.amplitude, best_frequency.power / (powers_average * 2. * log2(data.x.size())));
return output_tuple;
}

