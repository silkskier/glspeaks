#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm> //for sorting
#include <tuple>
#include <cmath>

#include "GLS_b.hpp"
#include <boost/spirit/include/qi.hpp>

using namespace std;


/*struct output_data {
	float power, amplitude, frequency;
	double sum_of_powers;
};*/


std::tuple<float, float, float> periodogram(float* frequencies_array, float step_size, int number_of_steps, std::filesystem::path in_file){


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


output_data best_frequency;

best_frequency = gls_b(t, y, e_y, length_of_data, number_of_steps, step_size, frequencies_array); //declares and fills a powers array //

float powers_average = best_frequency.sum_of_powers / number_of_steps; //calculates average power for the input data


std::tuple<float, float, float> output_tuple = make_tuple(best_frequency.frequency, best_frequency.amplitude, best_frequency.power / (powers_average * 2. * log2(length_of_data) ));

return output_tuple;
}
