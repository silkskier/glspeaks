#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <locale>

#include "../utils/readout.hpp"
#include "../../include/pdqsort.h"

#include "GLS/GLS_p_par.hpp"
#include "GLS/GLS_p_seq.hpp"
#include "../utils/vertex.hpp"

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
            break;
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

double res_0 = 12;
if (argc > 5 && ((mode != "-g" && argv[5][0] != '\0' && argv[3][0] != '-') || (mode == "-g" && argv[5][0] != '\0' && argv[5][0] != '-'))){res_0 = std::stoi(argv[5]);}


//creates frequency array
Grid grid; grid.generate(min_frequency, max_frequency, res_0);


float *amplitudes = (float *) malloc(grid.freq.size() * sizeof(float)), *powers = (float *) malloc(grid.freq.size() * sizeof(float)); //vectors storing step grid.freq.data() and powers for each frequency

//    for(int i = 0; i < grid.freq.size() ; i++) printf("%f, ", grid.freq[i]]); // prints grid.freq.data() vector

star data;
data.read(file);

uint length_of_data = data.x.size();

if (UseOpenMP){gls_p_par(data.x.data(), data.y.data(), data.dy.data(), length_of_data, grid.freq.size(), grid.fstep, grid.freq.data(), powers, amplitudes);}
else {gls_p_seq(data.x.data(), data.y.data(), data.dy.data(), length_of_data, grid.freq.size(), grid.fstep, grid.freq.data(), powers, amplitudes);}

//defines constants
const double normalization_constant = log2(length_of_data) * 2.;


std::vector<quad> output_data;

double powers_sum = 0;

for (unsigned int i=0; i < grid.freq.size(); i++){output_data.emplace_back(quad{i, powers[i], amplitudes[i], grid.freq[i]}); if(isnormal(powers[i])){powers_sum += powers[i];};};

double ony_by_powers_average = grid.freq.size() / powers_sum;

for (unsigned int i=0; i < grid.freq.size(); i++){output_data[i].power *= ony_by_powers_average;}



std::vector<quad> sorted_data;

for (unsigned int i=0; i < grid.freq.size(); i++){
if (output_data[i].power > 2){
sorted_data.push_back(output_data[i]);
}}


pdqsort_branchless(sorted_data.begin(), sorted_data.end(), [](const quad &a, const quad &b) {
    return a.power > b.power;
});

std::cout <<"\n" << "File: "<< files_location << "\n";

std::cout <<"\n " <<"f[1/d] 	P[d]	Amp[mag]	Power" << std::endl; //<< std::endl

unsigned int j = 0;
for (unsigned int i = 0; i < 20; i++) {
  for (unsigned int k = i; k <= i;) {
    if (sorted_data[j].power > output_data[sorted_data[j].index - 1].power && sorted_data[j].power > output_data[sorted_data[j].index + 1].power) {
    fmt::print(" {:.5f}\t{:.5f}\t{:.3f}\t{:.3f}\n", sorted_data[j].frequency, 1/sorted_data[j].frequency, sorted_data[j].amplitude, sorted_data[j].power / normalization_constant);
  j++;k++;
} else {j++;}

}}

std::cout << "\nNormalization constant: " << normalization_constant <<std::endl;
free(powers); free(amplitudes);
return;}
