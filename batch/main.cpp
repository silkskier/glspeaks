#include <iostream>
#include <iomanip>
#include <filesystem>
#include <vector>
#include <fstream>
#include <bits/stdc++.h>

#include <tuple>
#include <thread>
#include <sys/types.h>
#include <sys/wait.h>

#include "periodogram.hpp"

using namespace std;

bool filter(float frequency, float power, float min_value, float filter_range){
if(power < min_value){return false;}
else {return true;}
}



int main(int argc, char *argv[]){

//Message to print if there is not enough given arguments
 if(argc < 7){
     printf("\n Batch GLS period finder for variable stars, version 1.0\n");
     printf(" based on GLS algorithm by Mathias Zechmeister (https://github.com/mzechmeister/GLS)\n\n");
     printf(" Usage: GLS_batch <Path to catalog with data> <Min frequency> <Max frequency> <Resolution> <Max/Avg> <Filter range>\n\n");
     return 0;

}else{

//defines and calculates constants used for calculations
const std::string files_location = argv[1];
const float min_frequency = std::stof(argv[2]);
const float max_frequency = std::stof(argv[3]);
const float step_size = pow(0.5,std::stoi(argv[4]));
const float min_power = std::stof(argv[5]);
const float filter_range = std::stof(argv[6]);

std::cout << "\n" "Directory location: " << files_location << "\n";
std::cout << "Min frequency: " << min_frequency << "\n";
std::cout << "Max frequency: " << max_frequency << "\n";
std::cout << "Step size: " << step_size << "\n";
const int no_steps = (max_frequency - min_frequency)/step_size + 1;
std::cout << "Number of steps: " << no_steps << "\n";
std::cout << "Required avg/max power: " << min_power << "\n";
std::cout << "Filter range: " << filter_range << "\n";

//const int max_thread_number = std::thread::hardware_concurrency();

float *frequencies = (float *) malloc(no_steps * sizeof(float)); // creates frequencies and array
for(int step=0; step < no_steps;step++){frequencies[step] = min_frequency + step_size * step;} //fills frequency vector

//creates files array
std::vector<std::string> files;
auto directory_iterator = std::filesystem::directory_iterator(argv[1]);
int file_count = 0;
for (auto& entry : directory_iterator)
{files.push_back(entry.path());++file_count;}

std::cout <<"Number of files in directory: " << file_count << "\n" << std::endl;
// for(int i=0; i < files.size(); i++) std::cout << files.at(i) << ','; //prints list of files

string path = filesystem::path(files_location).parent_path(); string output_path = path + "/GLS_output.tsv"; ofstream output_file(output_path); //creates and opens output file

// calculates best-fitting period time and its power for each file using generalized Lomb-Scargle periodogram.

/*/
const int max_thread_number = std::thread::hardware_concurrency();
int i = 0;
for (i = 0; i < max_thread_number; ++i) {
    if (fork() == 0) {
int j;
for (j=i; j< file_count; j+= max_thread_number){auto [frequency, period, max_power] = periodogram(frequencies, step_size, no_steps, files.at(j));
if(filter(frequency, max_power, min_power, filter_range)==true) {output_file << files.at(j) << " " << frequency << " " << period << " " << max_power << std::endl;}}
exit(0);
}}
// wait all child processes
int status;
for (i = 0; i < max_thread_number; ++i) wait(&status);
/*/
const int files_per_cycle = 1024; const int number_of_cycles = ceil(file_count/files_per_cycle);

float best_frequencies[file_count]; float powers[file_count];

for (int j = 0; j< file_count; min(j+=files_per_cycle, file_count)){

std::cout <<std::fixed << std::setprecision(1) << 100*float(j)/float(file_count) << "%  " << std::flush;

#pragma omp parallel for
for (int i = j; i < min(j + files_per_cycle, file_count) ; i++)
{auto [frequency, period, max_power] = periodogram(frequencies, step_size, no_steps, files.at(i)); best_frequencies[i] = frequency, powers[i] =  max_power;}



for (int i = j; i < min(j + files_per_cycle, file_count); i++) if(filter(best_frequencies[i], powers[i], min_power, filter_range)==true) {output_file <<std::fixed << std::setprecision(6) << files.at(i) << " " << best_frequencies[i] << " " << 1/best_frequencies[i] << " " << powers[i] << std::endl;}}
cout << std::endl;
//*/


return 0;}
}
