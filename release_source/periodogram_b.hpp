#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm> //for sorting
#include <tuple>

#include "GLS_b.hpp"
#include <boost/spirit/include/qi.hpp>

using namespace std;


std::tuple<float, float, float> periodogram(float* frequencies_array, float step_size, int number_of_steps, std::string in_file){


std::vector<std::vector<float>> data; std::string line;
std::ifstream input_file(in_file);

    if (input_file) {
        std::string line;
        while (std::getline(input_file, line)) {
            std::vector<float> tempVec;
            std::string::const_iterator it = line.begin();
            std::string::const_iterator end = line.end();

            bool success = boost::spirit::qi::phrase_parse(it, end, *boost::spirit::qi::float_ >> *(boost::spirit::qi::lit(',') >> boost::spirit::qi::float_), boost::spirit::qi::space, tempVec);

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

unsigned int length_of_data = size(data); float t[length_of_data]; float y[length_of_data]; float e_y[length_of_data]; //declares variables used by GLS_periodogram
for (unsigned int i = 0; i < length_of_data; i++){t[i] = data[i][0], y[i] = data[i][1], e_y[i] = data[i][2];}; data.clear(); //transposes matrix and removes the original one

float *powers = (float *) malloc(number_of_steps * sizeof(float));
float *amplitudes = (float *) malloc(number_of_steps * sizeof(float));

 gls_b(t, y, e_y, length_of_data, number_of_steps, step_size, frequencies_array, powers, amplitudes); //declares and fills a powers array //

double powers_sum = 0; vector<float> powers_vector; //declares vector meant to store all the powers, but without any NaN's to enable further data analysis
for (unsigned int i=0; i < number_of_steps; i++){if(isnormal(powers[i])){powers_sum += powers[i]; powers_vector.push_back(powers[i]);}else{powers_vector.push_back(0);};}

float powers_average = powers_sum / number_of_steps; //calculates average power for the input data

unsigned int location = std::distance(powers_vector.begin(), std::max_element(powers_vector.begin(), powers_vector.end())); //finds location of the greatest power
//std::cout <<"\n"<< in_file << " " << frequencies_array[location] << " " << 1/frequencies_array[location] << " " << powers[location]/powers_average; //prints the output data

//float output[3] = {frequencies_array[location], 1/frequencies_array[location], powers[location]/powers_average}; //creates output array
std::tuple<float, float, float> output_tuple = make_tuple(frequencies_array[location], amplitudes[location], powers[location]/powers_average);
free(powers); free(amplitudes);
return output_tuple;
}
