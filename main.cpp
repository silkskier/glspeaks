#include <iostream>
#include <iomanip>
#include <filesystem>
#include <vector>
#include <fstream>
#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <boost/spirit/include/qi.hpp>

#include "my_shader.hpp"
#include <kompute/Kompute.hpp>


bool filter(float frequency, float power, float min_value, float filter_range){
if(power < min_value){return false;}
if(frequency < 1 + filter_range && frequency > 1 - filter_range){return false;}
if(frequency < 2 + filter_range && frequency > 2 - filter_range){return false;}
if(frequency < 3 + filter_range && frequency > 3 - filter_range){return false;}
if(frequency < 4 + filter_range && frequency > 4 - filter_range){return false;}
if(frequency < 5 + filter_range && frequency > 5 - filter_range){return false;}
if(frequency < 8 + filter_range && frequency > 8 - filter_range){return false;}
else {return true;}
}



int main(int argc, char *argv[]){

//Message to print if there is not enough given arguments
 if(argc < 8){
     printf("\n Batch GLS period finder for variable stars, version 1.0\n");
     printf(" based on GLS algorithm by Mathias Zechmeister (https://github.com/mzechmeister/GLS)\n\n");
     printf(" Usage: GLS_batch <Path to catalog with data> <Min frequency> <Max frequency> <Resolution> <Block size> <Max/Avg> <Filter range>\n\n");
     return 0;

}else{

//defines and calculates constants used for calculations
const std::string files_location = argv[1];
const float min_frequency = std::stof(argv[2]);
const float max_frequency = std::stof(argv[3]);
const float step_size = pow(0.5,std::stoi(argv[4]));
const int batch_size = std::stoi(argv[5]);
const float min_power = std::stof(argv[6]);
const float filter_range = std::stof(argv[7]);
const int no_steps = (max_frequency - min_frequency)/step_size + 1;
const int cells_per_file = ceil(float(no_steps)/1024);

std::cout << "\n" "Directory location: " << files_location << "\n";
std::cout << "Min frequency: " << min_frequency << "\n";
std::cout << "Max frequency: " << max_frequency << "\n";
std::cout << "Step size: " << step_size << "\n";
std::cout << "Number of steps: " << no_steps << "\n";
std::cout << "Batch size: " << batch_size << "\n";
std::cout << "Output cells per file: " << cells_per_file << "\n";
std::cout << "Required avg/max power: " << min_power << "\n";
std::cout << "Filter range: " << filter_range << "\n";

//const int max_thread_number = std::thread::hardware_concurrency();

std::vector<float> frequencies(no_steps); // creates vector
for(int step=0; step < no_steps;step++){frequencies[step] = min_frequency + step_size * step;} //fills frequencies array

//creates files array
std::vector<std::string> files;
auto directory_iterator = std::filesystem::directory_iterator(argv[1]);
int file_count = 0;
for (auto& entry : directory_iterator)
{files.push_back(entry.path());++file_count;}

std::cout <<"Number of files in directory: " << file_count << "\n" << std::endl;
// for(int i=0; i < files.size(); i++) std::cout << files.at(i) << ', '; //prints list of files
std::string path = std::filesystem::path(files_location).parent_path(); std::string output_path = path + "/GLS_output.tsv"; std::ofstream output_file(output_path); //creates and opens output file


const int number_of_cycles = ceil(file_count/batch_size);

//for (int k = 0; k< min(k+batch_size, file_count); k++){}

const int batch_number = 0; //do zmiany, będzie wynikać z wartości głównej pętli

int batch_length = std::min(batch_size, file_count - (batch_number * batch_size));


std::vector<std::vector<std::vector<float>>> all_data(batch_size); //creates temporary vector to enable multithreaded data read



#pragma omp parallel for //usunąć w przypadku HDD
for (int k = 0; k< batch_length; k++){
    std::vector<std::vector<float>> data;

    std::ifstream input_file(files[k]);

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

        all_data[k] = data;
    } else {
        std::cout << "file cannot be opened" << std::endl;}

    input_file.close();}


//generates breakpoint vector
std::vector<int> breakpoints(1, 0); //creates vectors for input data storage


//fills vectors with data
for (int k = 0; k < batch_length; k++){
breakpoints.push_back(breakpoints[k] + all_data[k].size());
            // std::cout<<files[k]<<" " <<breakpoints[k+1] - breakpoints[k]<<" "<<std::endl; //prints data length for each file to check the corectness of algorithm
}


std::vector<float> t(breakpoints.back()), y(breakpoints.back()), dy(breakpoints.back());  //creats data vectors

//fills vectors with data
#pragma omp parallel for
for (int k = 0; k < batch_length; k++){
for (int j = 0; j < breakpoints[k+1] - breakpoints[k] ; j++){
t[j + breakpoints[k]] =  all_data[k][j][0];
y[j + breakpoints[k]] =  all_data[k][j][1];
dy[j + breakpoints[k]] = all_data[k][j][2];
}}

all_data.clear(); //deletes 3D vector with data

int data_size = t.size();
            //std::cout<<t.size()<<" "<<y.size()<<" "<<dy.size()<<" "<<std::endl; - prints data size
for (int i = 1; i <= file_count;i++){
//std::cout<<breakpoints[i] - breakpoints[i-1]<<" "<<std::endl;
}

std::vector<int32_t> batch_info; //creates vector storing variables characterising batch
batch_info.push_back(data_size);
batch_info.push_back(128);
batch_info.push_back(batch_length);
batch_info.push_back(cells_per_file);

std::vector<float>batch(batch_length, 0);
std::vector<float>time_buffer(no_steps, 0);



std::vector<float> output_test(t.size(), 0); //creates long buffer vector


std::vector<float> output(batch_size * 3 , 0); //creates output vector // * cells_per_file


    kp::Manager mgr(1); // Selects GPU device at index 1

    std::shared_ptr<kp::TensorT<float>> tensorInT =
      mgr.tensor(t);

    std::shared_ptr<kp::TensorT<float>> tensorInY =
      mgr.tensor(y);

    std::shared_ptr<kp::TensorT<float>> tensorInDy =
      mgr.tensor(dy);

    std::shared_ptr<kp::TensorT<float>> tensorInF =
      mgr.tensor(frequencies);

    std::shared_ptr<kp::TensorT<float>> tensorShortBuffer =
mgr.tensor(batch);


std::shared_ptr<kp::TensorT<float>> tensorW =
mgr.tensor(output_test);
std::shared_ptr<kp::TensorT<float>> tensorWy =
mgr.tensor(output_test);

std::shared_ptr<kp::TensorT<float>> tensorYY =
mgr.tensor(time_buffer);
std::shared_ptr<kp::TensorT<float>> tensorOutput =
mgr.tensor(output);

std::shared_ptr<kp::TensorT<float>> tensorCosDx =
mgr.tensor(output_test);
std::shared_ptr<kp::TensorT<float>> tensorSinDx =
mgr.tensor(output_test);

std::shared_ptr<kp::TensorT<float>> tensorLongBuffer =
mgr.tensor(output_test);


std::shared_ptr<kp::TensorT<int32_t>> tensorBreakpoints =
mgr.tensorT(breakpoints);

std::shared_ptr<kp::TensorT<float>> tensorWsum =
mgr.tensor(batch);

std::shared_ptr<kp::TensorT<float>> tensorWnorm =
mgr.tensor(output_test);

std::shared_ptr<kp::TensorT<float>> tensorY =
mgr.tensor(batch);



    const std::vector<std::shared_ptr<kp::Tensor>> params = { tensorInT, tensorInY, tensorInDy,

tensorInF,

tensorShortBuffer,

tensorW, tensorWy, tensorYY, tensorOutput, tensorCosDx, tensorSinDx,

tensorLongBuffer,

tensorBreakpoints, tensorWsum, tensorWnorm, tensorY};

    const std::vector<uint32_t> shader = std::vector<uint32_t>(
      shader::MY_SHADER_COMP_SPV.begin(), shader::MY_SHADER_COMP_SPV.end());
    std::shared_ptr<kp::Algorithm> algo = mgr.algorithm(params, shader);

    mgr.sequence()
      ->record<kp::OpTensorSyncDevice>(params)
      ->record<kp::OpAlgoDispatch>(algo)
      ->record<kp::OpTensorSyncLocal>(params)
      ->eval();


//  for(int i=0; i<data.size(); i++) {for(int j=0; j<data[i].size(); j++) cout<<data[i][j]<<" "; cout<<endl;} // prints data array

    // prints output
/*
for (int i = 0; i < t.size(); i++){
std::cout <<y[i]<<" "<<dy[i]<<" "<<output_test[i]<<std::endl;
}
*/
std::vector<float> output_data = tensorShortBuffer->vector();


// prints all the data

std::cout <<"step size: " << step_size <<std::endl;

for (int i = 0; i < 10; i++){
std::cout<<t[i]<<" "<<output_data[i]<<std::endl;
}

std::cout << std::endl;


return 0;}
}
