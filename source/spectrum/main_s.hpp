#include <iostream>
#include <iomanip>
#include <string>
#include <filesystem>
#include <cmath>
#include <vector>
#include <fstream>
#include <sstream>
#include <locale>

#include "../utils/readout.hpp"
#include "../utils/grid.hpp"
#include "../utils/vertex.hpp"

#include "GLS/GLS_s.hpp"
#include "NCU/BNCU_s.hpp"
#include "CE/CE.hpp"

using namespace std;



void main_spectrum(int argc, char *argv[]){

std::locale::global(std::locale("C"));

std::cout << argc << argv[2] << std::endl;

if (argc < 3){return;} // || argv[2][0] == '\0' - breaks gui

std::string file = argv[2]; //source file
//defines variables used for calculations

std::string mode(argv[1]);
const std::string files_location = argv[2];

float min_frequency_temp = 0.003;
if (argc > 3 && ((mode != "-g" && argv[3][0]) != '\0') || (mode == "-g" && argv[3][0]) != '\0'){min_frequency_temp = std::stof(argv[3]);}
const float min_frequency = min_frequency_temp;

float max_frequency_temp = 10.0;
if (argc > 4 && ((mode != "-g" && argv[4][0]) != '\0') || (mode == "-g" && argv[4][0]) != '\0'){max_frequency_temp = std::stof(argv[4]);}
const float max_frequency = max_frequency_temp;

int res_0 = 12;
if (argc > 5 && ((mode != "-g" && argv[5][0]) != '\0') || (mode == "-g" && argv[5][0]) != '\0'){res_0 = std::stoi(argv[5]);}

std::cout << "\n" "File location: " << files_location << "\n";
std::cout << "Min frequency: " << min_frequency << "\n";
std::cout << "Max frequency: " << max_frequency << "\n";

grid grid; grid.generate(min_frequency, max_frequency, res_0);

std::cout << "Step size: " << grid.fstep << "\n";
std::cout << "Number of steps: " << grid.freq.size() << "\n";

double *powers = (double *) malloc(grid.freq.size() * sizeof(double));

star data;
data.read(file);


        //applies Generalized Lomb-Scargle periodogram for all the frequencies
unsigned int length_of_data = data.x.size();

gls_s(data.x.data(), data.y.data(), data.dy.data(), length_of_data, grid.freq.size(), grid.fstep, grid.freq.data(), powers);
//bncu_s(data, grid, powers);
//ce_s(data, grid, powers);

//        for (unsigned int i = 0; i < no_steps; i++) std::cout<< frequencies[i] <<" "<< powers[i] <<std::endl; //prints power for each frequency
// std::cout<< std::filesystem::path(file).filesystem::path::parent_path() <<std::endl; //prints input files directory

std::string output_path = std::filesystem::path(file).string() + "_output.tsv";



ofstream output_file(output_path); output_file.close(); //creates output file

auto out = fmt::output_file(output_path);
// output_file << "Frequency Power" "\n";

for (unsigned int i = 0; i < grid.freq.size(); i++){out.print(fmt::format("{:.5f}\t{:.5f}\n", grid.freq[i], powers[i]));}

 return;}

