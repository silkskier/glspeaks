#include <iostream>
#include <iomanip>
#include <string>
#include <filesystem>
#include <cmath>
#include <vector>
#include <fstream>
#include <sstream>

#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/karma.hpp>

#include "GLS_s.hpp"

using namespace std;



void main_spectrum(int argc, char *argv[]){


std::string file = argv[2]; //source file
//defines variables used for calculations
const std::string files_location = argv[2];
const double min_frequency = std::stof(argv[3]);
const double max_frequency = std::stof(argv[4]);

double step_size_0 = pow(0.5, 12);
if (argc > 5){step_size_0 = pow(0.5,std::stoi(argv[5]));}

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


std::vector<std::vector<double>> data; std::string line;
std::ifstream input_file(file);

    if (input_file) {
        std::string line;
        while (std::getline(input_file, line)) {
            std::vector<double> tempVec;
            std::string::const_iterator it = line.begin();
            std::string::const_iterator end = line.end();

            bool success = boost::spirit::qi::phrase_parse(it, end, *boost::spirit::qi::double_ >> *(boost::spirit::qi::lit(',') >> boost::spirit::qi::double_), boost::spirit::qi::space, tempVec);

            if (success && it == end) {
                data.emplace_back(tempVec);
            } else {
                std::cout << "Parsing failed" << std::endl;
                break;
            }}
    } else {
        std::cout << "file cannot be opened" << std::endl;}

    input_file.close();
//  for(unsigned int i=0; i<data.size(); i++) {for(unsigned int j=0; j<data[i].size(); j++) cout<<data[i][j]<<" "; cout<<endl;} // prints data array



        //applies Generalized Lomb-Scargle periodogram for all the frequencies
unsigned int length_of_data = size(data);
double t[length_of_data]; double y[length_of_data]; double e_y[length_of_data];
for (unsigned int i = 0; i < length_of_data; i++) t[i] = data[i][0], y[i] = data[i][1], e_y[i] = data[i][2]; //transpose vectors
data.clear();
//  for (unsigned int i = 0; i < length_of_data; i++) std::cout<< t[i] <<" "<< y[i] <<" "<< e_y[i] <<std::endl; //prints transposed data array

gls_s(t, y, e_y, length_of_data, no_steps, step_size_0, frequencies, powers);

//        for (unsigned int i = 0; i < no_steps; i++) std::cout<< frequencies[i] <<" "<< powers[i] <<std::endl; //prints power for each frequency
// std::cout<< std::filesystem::path(file).filesystem::path::parent_path() <<std::endl; //prints input files directory
string path = std::filesystem::path(file); string output_path = path + "_output.tsv";




  std::vector<std::string> output_string(no_steps);

	#pragma omp parallel for
  for (unsigned int i = 0; i < no_steps; i++) {

  boost::spirit::karma::generate
	(std::back_inserter(output_string[i]),
	boost::spirit::double_(frequencies[i])
	<< "\t"
	<< boost::spirit::double_(powers[i]));
}


ofstream output_file(output_path); //creates and opens output file

// output_file << "Frequency Power" "\n";

for (unsigned int i = 0; i < no_steps; i++) output_file << output_string[i] << "\n";

}
