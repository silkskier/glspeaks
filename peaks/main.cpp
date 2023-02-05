#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <fstream>
#include <sstream>
#include <tuple>
#include <bits/stdc++.h> //for sorting

#include "GLS.hpp"

using namespace std;

struct quad {
	int index;
	float power, amplitude, frequency;
};

int main(int argc, char *argv[]){

//gls() - Generalized Lomb-Scargle
if(argc < 5){
     printf("\n Generalized Lomb-Scargle periodogram for variable stars, version of 2022\n");
     printf(" based on C version of GLS by Mathias Zechmeister (https://github.com/mzechmeister/GLS).\n");
     printf(" Computing of amplitude spectrum for time series data.\n\n");
     printf(" Usage: GLS <Path to file> <Min frequency> <Max frequency> <Resolution>\n\n");
     return 0;

}else if(stof(argv[2]) <= 0){
     printf("\n Error; Starting frequency must be greater, than 0\n");
     return 1;

}else if (std::stoi(argv[4]) > 23) {printf(" Error: Step sizes smaller than 2^-23 unsupported due to limitations of 32-bit floats"); return 1;

}else{


std::string file = argv[1]; //source file
//defines variables used for calculations
const std::string files_location = argv[1];
const float min_frequency = std::stof(argv[2]);
const float max_frequency = std::stof(argv[3]);
const float step_size_0 = pow(0.5,std::stoi(argv[4]));

    //std::cout << "\n" "Directory location: " << files_location << "\n";
    //std::cout << "Min frequency: " << min_frequency << "\n";
    //std::cout << "Max frequency: " << max_frequency << "\n";
    //std::cout << "Step size: " << step_size_0 << "\n";

//creates frequency array
const int no_steps = (max_frequency - min_frequency)/step_size_0 + 1;
    //std::cout << "Number of steps: " << no_steps << "\n";

float *frequencies = (float *) malloc(no_steps * sizeof(float)),
*powers = (float *) malloc(no_steps * sizeof(float)),  *amplitudes = (float *) malloc(no_steps * sizeof(float)); //vectors storing step frequencies and powers for each frequency

for(int step=0; step < no_steps;step++){frequencies[step] = min_frequency + step_size_0 * step;} //fills frequency vector
//    for(int i = 0; i < no_steps ; i++) printf("%f, ", frequencies[i]); // prints frequencies vector


std::vector<std::vector<float>> data;

std::string line;
    float word;
    std::ifstream input_file(file);
    if(input_file){
        while(getline(input_file, line, '\n')){
            //create a temporary vector that will contain all the columns
            std::vector<float> tempVec;
            std::istringstream ss(line);
            //read float by float
            while(ss >> word){
                //add the float to the temporary vector
                tempVec.push_back(word);}
            //add floats from the current line has been added to the temporary vector
            data.emplace_back(tempVec);}
}else{
std::cout<<"file cannot be opened"<<std::endl;}
    input_file.close();
//  for(int i=0; i<data.size(); i++) {for(int j=0; j<data[i].size(); j++) cout<<data[i][j]<<" "; cout<<endl;} // prints data array



        //applies Generalized Lomb-Scargle periodogram for all the frequencies
int length_of_data = size(data);
float t[length_of_data]; float y[length_of_data]; float e_y[length_of_data];
for (int i = 0; i < length_of_data; i++) t[i] = data[i][0], y[i] = data[i][1], e_y[i] = data[i][2]; //transpose vectors
data.clear();
//  for (int i = 0; i < length_of_data; i++) std::cout<< t[i] <<" "<< y[i] <<" "<< e_y[i] <<std::endl; //prints transposed data array

gls(t, y, e_y, length_of_data, no_steps, step_size_0, frequencies, powers, amplitudes);

//        for (int i = 0; i < no_steps; i++) std::cout<< frequencies[i] <<" "<< powers[i] <<std::endl; //prints power for each frequency
//  //prints input files directory
                                            //string path = filesystem::path(file); string output_path = path + "_output.csv"; ofstream output_file(output_path); //creates and opens output file

// output_file << "Frequency Power" "\n";
//for (int i = 0; i < no_steps; i++) output_file << frequencies[i] <<" "<< powers[i] <<std::endl; //prints frequencies and corresponding frequencies to a text file

std::vector<quad> output_data;

float powers_sum = 0;

for (int i=0; i < no_steps; i++){output_data.emplace_back(quad{i, powers[i], amplitudes[i], frequencies[i]}); if(isnormal(powers[i])){powers_sum += powers[i];};};

float ony_by_powers_average = no_steps / powers_sum;

for (int i=0; i < no_steps; i++){output_data[i].power *= ony_by_powers_average;}



std::vector<quad> sorted_data;

for (int i=0; i < no_steps; i++){
if (output_data[i].power > 2){
sorted_data.push_back(output_data[i]);
}}

// float average_power = sum_of_powers/no_steps;

//frequencies.clear(), powers.clear(); // removes lists from memory - doesn't work on lists
std::sort(sorted_data.begin(), sorted_data.end(), [](const quad &a, const quad &b) {
    return a.power > b.power;
});

std::cout <<"\n" << "File: "<< files_location << "\n";

std::cout <<"\n  " <<"f[1/d]	P[d]		Amp		max/avg" << std::endl; //<< std::endl
std::cout << std::fixed << std::setprecision(8); //sets cout's precision

int j = 0;
for (int i = 0; i < 20; i++) {
  for (int k = i; k <= i;) {
    if (sorted_data[j].power > output_data[sorted_data[j].index - 1].power && sorted_data[j].power > output_data[sorted_data[j].index + 1].power) {

    std::cout << "  " << sorted_data[j].frequency << "	" << 1/sorted_data[j].frequency << "	" << sorted_data[j].amplitude << "	" << sorted_data[j].power << std::endl;
  j++;k++;
} else {j++;}

}}
free(frequencies); free(powers); free(amplitudes);
}return 0;}
