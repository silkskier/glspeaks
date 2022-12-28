#include <iostream>
#include <string>
#include <filesystem>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <vector>
#include <fstream>
#include <bits/stdc++.h>

using namespace std;

int main(int argc, char *argv[]){

//Message to print if there is not enough given arguments
 if(argc < 5){
     printf("\n FNPEAKS_batch for Linux, version of 2022\n");
     printf(" by W. Hebisch, Z. Kolaczkowski and G. Kopacki.\n");
     printf(" Computing of amplitude spectrum for time series data.\n\n");
     printf(" Usage: fnpeaks <Path to catalog with data files> <Min frequency> <Max frequency> <Resolution>\n\n");
     return 0;

}else{

//defines and calculates constants used for calculations
const std::string files_location = argv[1];
const float min_frequency = std::stof(argv[2]);
const float max_frequency = std::stof(argv[3]);
const float step_size_0 = pow(0.5,std::stoi(argv[4]));
std::cout << "\n" "Directory location: " << files_location << "\n";
std::cout << "Min frequency: " << min_frequency << "\n";
std::cout << "Max frequency: " << max_frequency << "\n";
std::cout << "Step size: " << step_size_0 << "\n";
const int no_steps = (max_frequency - min_frequency)/step_size_0 + 1;
std::cout << "Number of steps: " << no_steps << "\n";

{ // creates frequencies and powers array
std::vector<vector<float>> steps; //declares array storing step frequencies ([0]) and powers ([1]) for each frequency
std::vector<float> frequencies; steps.push_back(frequencies); //declares frequency vector
std::vector<float> powers; steps.push_back(powers); //declares powers vector
for(int step=0; step < no_steps;step++){steps[0].push_back(min_frequency + step_size_0 * step);}} //fills frequency vector
//    for(int i = 0; i < no_steps ; i++) printf("%f, ", steps[0][i]); // prints frequencies vector


//creates files array
std::vector<std::string> files;
auto directory_iterator = std::filesystem::directory_iterator(argv[1]);
int file_count = 0;

for (auto& entry : directory_iterator)
{files.push_back(entry.path());++file_count;}
std::cout <<"Number of files in directory: " << file_count << "\n";
// for(int i=0; i < files.size(); i++) std::cout << files.at(i) << ','; //prints list of files


// calculates best-fitting period time and its power for each file using generalized Lomb-Scargle periodogram.

return 0;}
}
