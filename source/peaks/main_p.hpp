#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm> //for sorting
#include <locale>

#include "../extras/readout.hpp"
#include "../../include/pdqsort.h"

#include "GLS/GLS_p_par.hpp"
#include "GLS/GLS_p_seq.hpp"
#include "../extras/vertex.hpp"

bool UseOpenMP = true;

using namespace std;

struct quad {
	unsigned int index;
	double power, amplitude, frequency;
};

void main_peaks(int argc, char *argv[]){

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--OpenMP=false" || arg == "--UseOpenMP=false" || arg == "-nomp") {
            UseOpenMP = false;
            break; // No need to continue checking once found
        }}

std::locale::global(std::locale("C"));

if (argc < 3){return;} // || argv[3][0] //breaks gui

std::string mode(argv[1]);
std::string file = argv[2]; //source file
//defines variables used for calculations
const std::string files_location = argv[2];


float min_frequency_temp = 0.003;
if (argc > 3 && ((mode != "-g" && argv[3][0] != '\0' && argv[3][0] != '-') || (mode == "-g" && argv[3][0] != '\0' && argv[3][0] != '-'))) min_frequency_temp = std::stof(argv[3]);
const float min_frequency = min_frequency_temp;

float max_frequency_temp = 10.0;
if (argc > 4 && ((mode != "-g" && argv[4][0] != '\0' && argv[3][0] != '-') || (mode == "-g" && argv[4][0] != '\0' && argv[4][0] != '-'))){max_frequency_temp = std::stof(argv[4]);}
const float max_frequency = max_frequency_temp;

double step_size_0 = pow(0.5, 12);
if (argc > 5 && ((mode != "-g" && argv[5][0] != '\0' && argv[3][0] != '-') || (mode == "-g" && argv[5][0] != '\0' && argv[5][0] != '-'))){step_size_0 = pow(0.5,std::stoi(argv[5]));}

    //std::cout << "\n" "Directory location: " << files_location << "\n";
    //std::cout << "Min frequency: " << min_frequency << "\n";
    //std::cout << "Max frequency: " << max_frequency << "\n";
    //std::cout << "Step size: " << step_size_0 << "\n";

//creates frequency array
const unsigned int no_steps = (max_frequency - min_frequency)/step_size_0 + 1;
    //std::cout << "Number of steps: " << no_steps << "\n";

double *frequencies = (double *) malloc(no_steps * sizeof(double)),
*powers = (double *) malloc(no_steps * sizeof(double)),  *amplitudes = (double *) malloc(no_steps * sizeof(double)); //vectors storing step frequencies and powers for each frequency

for(unsigned int step=0; step < no_steps;step++){frequencies[step] = min_frequency + step_size_0 * step;} //fills frequency vector
//    for(int i = 0; i < no_steps ; i++) printf("%f, ", frequencies[i]); // prints frequencies vector

star data;
data.read(file);

uint length_of_data = data.x.size();

if (UseOpenMP){gls_p_par(data.x.data(), data.y.data(), data.dy.data(), length_of_data, no_steps, step_size_0, frequencies, powers, amplitudes);}
else {gls_p_seq(data.x.data(), data.y.data(), data.dy.data(), length_of_data, no_steps, step_size_0, frequencies, powers, amplitudes);}

//defines constants
const double normalization_constant = log2(length_of_data) * 2.;


std::vector<quad> output_data;

double powers_sum = 0;

for (unsigned int i=0; i < no_steps; i++){output_data.emplace_back(quad{i, powers[i], amplitudes[i], frequencies[i]}); if(isnormal(powers[i])){powers_sum += powers[i];};};

double ony_by_powers_average = no_steps / powers_sum;

for (unsigned int i=0; i < no_steps; i++){output_data[i].power *= ony_by_powers_average;}



std::vector<quad> sorted_data;

for (unsigned int i=0; i < no_steps; i++){
if (output_data[i].power > 2){
sorted_data.push_back(output_data[i]);
}}

// double average_power = sum_of_powers/no_steps;

//frequencies.clear(), powers.clear(); // removes lists from memory - doesn't work on lists
std::sort(sorted_data.begin(), sorted_data.end(), [](const quad &a, const quad &b) {
    return a.power > b.power;
});

std::cout <<"\n" << "File: "<< files_location << "\n";

std::cout <<"\n " <<"f[1/d] 	P[d]	Amp[mag]	Power" << std::endl; //<< std::endl
//std::cout << std::fixed;  std::cout<< std::setprecision(5); //sets cout's precision

unsigned int j = 0;
for (unsigned int i = 0; i < 20; i++) {
  for (unsigned int k = i; k <= i;) {
    if (sorted_data[j].power > output_data[sorted_data[j].index - 1].power && sorted_data[j].power > output_data[sorted_data[j].index + 1].power) {
    fmt::print(" {:.5f}\t{:.5f}\t{:.3f}\t{:.3f}\n", sorted_data[j].frequency, 1/sorted_data[j].frequency, sorted_data[j].amplitude, sorted_data[j].power / normalization_constant);
  j++;k++;
} else {j++;}

}}

std::cout << "\nNormalization constant: " << normalization_constant <<std::endl;
free(frequencies); free(powers); free(amplitudes);
return;}
