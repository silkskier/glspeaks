#include <iostream>
#include <string>
#include <filesystem>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <vector>
#include <fstream>
#include <sstream>

#include "GLS.hpp"

using namespace std;



int main(int argc, char *argv[]){

std::string file = argv[1]; //source file


//gls() - Generalized Lomb-Scargle
if(argc < 5){
     printf("\n FNPEAKS_batch for Linux, version of 2022\n");
     printf(" by W. Hebisch, Z. Kolaczkowski and G. Kopacki.\n");
     printf(" Computing of amplitude spectrum for time series data.\n\n");
     printf(" Usage: fnpeaks <Path to catalog with data files> <Min frequency> <Max frequency> <Resolution>\n\n");
     return 0;

}else{

//defines variables used for calculations
const std::string files_location = argv[1];
const double min_frequency = std::stof(argv[2]);
const double max_frequency = std::stof(argv[3]);
const double step_size_0 = pow(0.5,std::stoi(argv[4]));

std::cout << "\n" "Directory location: " << files_location << "\n";
std::cout << "Min frequency: " << min_frequency << "\n";
std::cout << "Max frequency: " << max_frequency << "\n";
std::cout << "Step size: " << step_size_0 << "\n";

//creates frequency array
const int no_steps = (max_frequency - min_frequency)/step_size_0 + 1;
std::cout << "Number of steps: " << no_steps << "\n";

double *frequencies = (double *) malloc(no_steps * sizeof(double)), *powers = (double *) malloc(no_steps * sizeof(double)); //vectors storing step frequencies and powers for each frequency
for(int step=0; step < no_steps;step++){frequencies[step] = min_frequency + step_size_0 * step;} //fills frequency vector
//    for(int i = 0; i < no_steps ; i++) printf("%f, ", frequencies[i]); // prints frequencies vector


std::vector<std::vector<double>> data;

std::string line;
    double word;
    std::ifstream input_file(file);
    if(input_file){
        while(getline(input_file, line, '\n')){
            //create a temporary vector that will contain all the columns
            std::vector<double> tempVec;
            std::istringstream ss(line);
            //read double by double
            while(ss >> word){
                //add the double to the temporary vector
                tempVec.push_back(word);}
            //add doubles from the current line has been added to the temporary vector
            data.emplace_back(tempVec);}
}else{
std::cout<<"file cannot be opened"<<std::endl;}
    input_file.close();
//  for(int i=0; i<data.size(); i++) {for(int j=0; j<data[i].size(); j++) cout<<data[i][j]<<" "; cout<<endl;} // prints data array



        //applies Generalized Lomb-Scargle periodogram for all the frequencies
int length_of_data = size(data);
double t[length_of_data]; double y[length_of_data]; double e_y[length_of_data];
for (int i = 0; i < length_of_data; i++) t[i] = data[i][0], y[i] = data[i][1], e_y[i] = data[i][2]; //transpose vectors
data.clear();
//  for (int i = 0; i < length_of_data; i++) std::cout<< t[i] <<" "<< y[i] <<" "<< e_y[i] <<std::endl; //prints transposed data array

gls(t, y, e_y, length_of_data, no_steps, step_size_0, frequencies, powers);

//        for (int i = 0; i < no_steps; i++) std::cout<< frequencies[i] <<" "<< powers[i] <<std::endl; //prints power for each frequency
// std::cout<< filesystem::path(file).filesystem::path::parent_path() <<std::endl; //prints input files directory
string path = filesystem::path(file).filesystem::path::parent_path(); string output_path = path + "/GLS_output.csv"; ofstream output_file(output_path); //creates and opens output file

// output_file << "Frequency Power" "\n";
for (int i = 0; i < no_steps; i++) output_file << frequencies[i] <<" "<< powers[i] <<std::endl;

}return 0;}
