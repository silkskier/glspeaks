#include <iostream>
#include <iomanip>
#include <string>
#include <filesystem>
#include <cmath>
#include <vector>
#include <fstream>
#include <sstream>
#include <locale>

#include "../extras/readout.hpp"

#include "GLS/GLS_s.hpp"

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

double step_size_0 = pow(0.5, 12);
if (argc > 5 && ((mode != "-g" && argv[5][0]) != '\0') || (mode == "-g" && argv[5][0]) != '\0'){step_size_0 = pow(0.5,std::stoi(argv[5]));}

std::cout << "\n" "File location: " << files_location << "\n";
std::cout << "Min frequency: " << min_frequency << "\n";
std::cout << "Max frequency: " << max_frequency << "\n";
std::cout << "Step size: " << step_size_0 << "\n";


//creates frequency array
const unsigned int no_steps = (max_frequency - min_frequency)/step_size_0 + 1;
std::cout << "Number of steps: " << no_steps << "\n";

double *frequencies = (double *) malloc(no_steps * sizeof(double)), *powers = (double *) malloc(no_steps * sizeof(double)); //vectors storing step frequencies and powers for each frequency
for(unsigned int step=0; step < no_steps;step++){frequencies[step] = min_frequency + step_size_0 * step;} //fills frequency vector
//    for(unsigned int i = 0; i < no_steps ; i++) printf("%f, ", frequencies[i]); // prints frequencies vector


star data;
data.read(file);


        //applies Generalized Lomb-Scargle periodogram for all the frequencies
unsigned int length_of_data = data.x.size();

gls_s(data.x.data(), data.y.data(), data.dy.data(), length_of_data, no_steps, step_size_0, frequencies, powers);

//        for (unsigned int i = 0; i < no_steps; i++) std::cout<< frequencies[i] <<" "<< powers[i] <<std::endl; //prints power for each frequency
// std::cout<< std::filesystem::path(file).filesystem::path::parent_path() <<std::endl; //prints input files directory

std::string output_path = std::filesystem::path(file).string() + "_output.tsv";



ofstream output_file(output_path); output_file.close(); //creates output file

auto out = fmt::output_file(output_path);
// output_file << "Frequency Power" "\n";

for (unsigned int i = 0; i < no_steps; i++){out.print(fmt::format("{:.5f}\t{:.5f}\n", frequencies[i], powers[i]));}

 return;}

