#include <iostream>
#include <iomanip>
#include <filesystem>
#include <vector>
#include <fstream>
#include <tuple>
#include <thread>
#include <atomic>
#include <chrono>

#include <QProgressDialog>
#include <QCoreApplication>

#include <boost/spirit/include/karma.hpp>
#include <boost/spirit/include/karma_real.hpp>

#include "periodogram_b.hpp"

using namespace std;

std::atomic<int> progressValue(0), timeLeft(0);

bool filter(float frequency, float power, float min_value, float filter_range, float amplitude, float min_amplitude, float max_amplitude){

    if (power < min_value) return false; //powers filter

    if (amplitude < min_amplitude || amplitude > max_amplitude) return false; //amplitudes filter

    float filter_frequencies[] = {1, 2, 3, 4, 5, 8};
    unsigned int num_frequencies = sizeof(filter_frequencies) / sizeof(filter_frequencies[0]);
    if (frequency == 0) return 0;
    for (unsigned int i = 0; i < num_frequencies; ++i)
    {if (frequency > filter_frequencies[i] - (filter_range / 2) && frequency < filter_frequencies[i] + (filter_range / 2)) return false;}//frequencies filter

    return true;
}



void main_batch(int argc, char *argv[]){


//defines and calculates constants used for calculations
const std::string files_location = argv[2];

if (argc < 3){return;} // || argv[3][0] == '\0' - breaks gui

float min_frequency_temp = 0.003;
if (argc > 3 && argv[3][0] != '\0'){min_frequency_temp = std::stof(argv[3]);}
const float min_frequency = min_frequency_temp;

float max_frequency_temp = 10.0;
if (argc > 4 && argv[4][0] != '\0'){max_frequency_temp = std::stof(argv[4]);}
const float max_frequency = max_frequency_temp;


float step_size = pow(0.5, 12);
if (argc > 5 && argv[5][0] != '\0'){step_size = pow(0.5,std::stoi(argv[5]));}
float min_power = 1;
if (argc > 6 && argv[6][0] != '\0'){min_power = std::stof(argv[6]);}
float filter_range = 0.04;
if (argc > 7 && argv[7][0] != '\0'){filter_range = std::stof(argv[7]);}
float min_amplitude = 0.01;
if (argc > 8 && argv[8][0] != '\0'){min_amplitude = std::stof(argv[8]);}
float max_amplitude = 8;
if (argc > 9 && argv[9][0] != '\0'){max_amplitude = std::stof(argv[9]);}

std::cout << "\n" "Directory location: " << files_location << "\n";
std::cout << "Min frequency: " << min_frequency << "\n";
std::cout << "Max frequency: " << max_frequency << "\n";
std::cout << "Step size: " << step_size << "\n";
const unsigned int no_steps = (max_frequency - min_frequency)/step_size + 1;
std::cout << "Number of steps: " << no_steps << "\n";
std::cout << "Required avg/max power: " << min_power << "\n";
std::cout << "Frequency filter range: " << filter_range << "\n";
std::cout << "Amplitudes range: " << "[" << min_amplitude << " , " << max_amplitude << "]" << "\n";

//const int max_thread_number = std::thread::hardware_concurrency();

float *frequencies = (float *) malloc(no_steps * sizeof(float)); // creates frequencies and array
for(unsigned int step=0; step < no_steps;step++){frequencies[step] = min_frequency + step_size * step;} //fills frequency vector

//creates files array
std::vector<std::filesystem::path> files;
auto directory_iterator = std::filesystem::directory_iterator(argv[2]);
unsigned int file_count = 0;
for (auto& entry : directory_iterator)
{files.push_back(entry.path());++file_count;}

std::cout <<"Number of files in directory: " << file_count << "\n" << std::endl;
// for(unsigned int i=0; i < files.size(); i++) std::cout << files.at(i) << ','; //prints list of files

std::filesystem::path path = filesystem::path(files_location).parent_path(); string output_path = path.string() + "/GLS_output.tsv"; ofstream output_file(output_path); //creates and opens output file

//output_file << "<path_to_file>\t<frequency>\t<period>\t<amplitude>\t<avg/max>" << std::endl;

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

const unsigned int files_per_cycle = 1024; const unsigned int number_of_cycles = ceil(file_count/files_per_cycle);
float best_frequencies[file_count]; float powers[file_count]; float amplitudes[file_count];

std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
std::chrono::steady_clock::time_point currentTime;
std::chrono::duration<double> elapsedTime;



for (unsigned int j = 0; j< file_count; min(j+=files_per_cycle, file_count)){ //loops code execution

if (string(argv[1]) == "-g") {
        currentTime = std::chrono::steady_clock::now();
        elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime);
        progressValue.store(static_cast<int>(1000 * static_cast<float>(j) / static_cast<float>(file_count)));
        timeLeft.store(static_cast<int>(elapsedTime.count() * ((static_cast<float>(file_count) - static_cast<float>(j)) / static_cast<float>(j))));}
else {std::cout << std::fixed << std::setprecision(1) << "\r" << 100*float(j)/float(file_count) << "% complete" << std::flush;}



#pragma omp parallel for
for (unsigned int i = j; i < min(j + files_per_cycle, file_count) ; i++)
{auto [frequency, amplitude, max_power] = periodogram(frequencies, step_size, no_steps, files[i]); best_frequencies[i] = frequency, amplitudes[i] =  amplitude, powers[i] =  max_power;}



//for (unsigned int i = j; i < min(j + files_per_cycle, file_count); i++) if(filter(best_frequencies[i], powers[i], min_power, filter_range, amplitudes[i], min_amplitude, max_amplitude)==true)
//{output_file <<std::fixed << std::setprecision(8) << files.at(i) << "	" << best_frequencies[i] << "	" << 1/best_frequencies[i] << "	" << amplitudes[i] << "	" << powers[i] << std::endl;}

std::vector<std::string> output_string(files_per_cycle);
#pragma omp parallel for
for (unsigned int i = j; i < min(j + files_per_cycle, file_count); i++) if(filter(best_frequencies[i], powers[i], min_power, filter_range, amplitudes[i], min_amplitude, max_amplitude)==true){


  boost::spirit::karma::generate
	(std::back_inserter(output_string[i-j]),
	boost::spirit::stream(files[i])
	<< "\t"<< boost::spirit::float_(best_frequencies[i])
	<< "\t"<< boost::spirit::float_(1/best_frequencies[i])
	<< "\t"<< boost::spirit::float_(amplitudes[i])
	<< "\t"<< boost::spirit::float_(powers[i]));}

	output_string.erase(remove(output_string.begin(), output_string.end(), ""), output_string.end());

for (unsigned int i = 0; i < output_string.size(); i++) output_file << output_string[i] <<std::endl; output_string.clear();
}

if (string(argv[1]) == "-g") {progressValue.store(static_cast<int>(1000)), timeLeft.store(static_cast<int>(0));}
else {std::cout << "\r" << "Complete" << std::endl;}
}
