#include <iostream>
#include <vector>
#include <fstream>
#include <bits/stdc++.h> //for sorting
#include <tuple>

#include "GLS.hpp"

using namespace std;


std::tuple<float, float, float> periodogram(float* frequencies_array, float step_size, int number_of_steps, std::string in_file){


std::vector<std::vector<float>> data; std::string line;float word;
std::ifstream input_file(in_file);
    if(input_file){
    while(getline(input_file, line, '\n')){ //creates a temporary vector that will contain all the columns
    std::vector<float> tempVec; std::istringstream ss(line); //read float by float
    while(ss >> word){tempVec.push_back(word);} //adds the float to the temporary vector
    data.emplace_back(tempVec);} //add floats from the current line has been added to the temporary vector
}else{std::cout<<"file cannot be opened"<<std::endl;} input_file.close();
//  for(int i=0; i<data.size(); i++) {for(int j=0; j<data[i].size(); j++) cout<<data[i][j]<<" "; cout<<endl;} // prints data array

int length_of_data = size(data); float t[length_of_data]; float y[length_of_data]; float e_y[length_of_data]; //declares variables used by GLS_periodogram
for (int i = 0; i < length_of_data; i++){t[i] = data[i][0], y[i] = data[i][1], e_y[i] = data[i][2];}; data.clear(); //transposes matrix and removes the original one

float *powers = (float *) malloc(number_of_steps * sizeof(float)); gls(t, y, e_y, length_of_data, number_of_steps, step_size, frequencies_array, powers); //declares and fills a powers array

float powers_sum = 0; vector<float> powers_vector; //declares vector meant to store all the powers, but without any NaN's to enable further data analysis
for (int i=0; i < number_of_steps; i++){if(isnormal(powers[i])){powers_sum += powers[i]; powers_vector.push_back(powers[i]);}else{powers_vector.push_back(0);};}
float powers_average = powers_sum / number_of_steps; //calculates average power for the input data

int location = std::distance(powers_vector.begin(), std::max_element(powers_vector.begin(), powers_vector.end())); //finds location of the greatest power

//std::cout <<"\n"<< in_file << " " << frequencies_array[location] << " " << 1/frequencies_array[location] << " " << powers[location]/powers_average; //prints the output data

//float output[3] = {frequencies_array[location], 1/frequencies_array[location], powers[location]/powers_average}; //creates output array
return {frequencies_array[location], 1/frequencies_array[location], powers[location]/powers_average};}
