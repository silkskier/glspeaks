#include <iostream>
#include <iomanip>
#include <filesystem>
#include <vector>
#include <fstream>
#include <tuple>
#include <thread>
#include <atomic>
#include <chrono>
#include <locale>

#include <QProgressDialog>
#include <QCoreApplication>

#include "../extras/grid.hpp"
#include "periodogram_b.hpp"

using namespace std;

std::atomic<float> progressValue(0);
std::atomic<int> timeLeft(0), filesComputed(0);

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

void printProgress(int max_progress, const std::chrono::steady_clock::time_point startTime) {
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (filesComputed.load() == max_progress) {
            return;
        }
        int currentFilesComputed = filesComputed.load();
        float progress = 100. * static_cast<float>(currentFilesComputed) / static_cast<float>(max_progress);
        std::chrono::duration<double> elapsedTime = std::chrono::steady_clock::now() - startTime;
        float localTimeLeft = static_cast<float>(elapsedTime.count()) * (float(max_progress) - float(currentFilesComputed)) / float(currentFilesComputed);
        progressValue.store(10 * progress);
        timeLeft.store(int(localTimeLeft));
        std::cout << std::fixed << std::setprecision(2) << "\r" << progress << "% complete, " << localTimeLeft << " seconds left." << std::flush;
    }
}

void main_batch(int argc, char *argv[]){

std::locale::global(std::locale("C"));

std::string mode(argv[1]);


//defines and calculates constants used for calculations
const std::string files_location = argv[2];

if (argc < 3){return;} // || argv[3][0] == '\0' - breaks gui


float min_frequency_temp = 0.003;
if (argc > 3 && ((mode != "-g" && argv[3][0]) != '\0') || (mode == "-g" && argv[3][0]) != '\0'){min_frequency_temp = std::stof(argv[3]);}
const float min_frequency = min_frequency_temp;

float max_frequency_temp = 10.0;
if (argc > 4 && ((mode != "-g" && argv[4][0]) != '\0') || (mode == "-g" && argv[4][0]) != '\0'){max_frequency_temp = std::stof(argv[4]);}
const float max_frequency = max_frequency_temp; //0

float resolution = 12;
if (argc > 5 && ((mode != "-g" && argv[5][0]) != '\0') || (mode == "-g" && argv[5][0]) != '\0'){resolution = std::stoi(argv[5]);}

float min_power_temp = 1;
if (argc > 6 && ((mode != "-g" && argv[6][0]) != '\0') || (mode == "-g" && argv[6][0]) != '\0'){min_power_temp = std::stof(argv[6]);}

const float min_power = min_power_temp;

float filter_range = 0.04;
if (argc > 7 && ((mode != "-g" && argv[7][0]) != '\0') || (mode == "-g" && argv[7][0]) != '\0'){filter_range = std::stof(argv[7]);}

float min_amplitude = 0.01;
if (argc > 8 && ((mode != "-g" && argv[8][0]) != '\0') || (mode == "-g" && argv[8][0]) != '\0'){min_amplitude = std::stof(argv[8]);}

float max_amplitude = 8;
if (argc > 9 && ((mode != "-g" && argv[9][0]) != '\0') || (mode == "-g" && argv[9][0]) != '\0'){max_amplitude = std::stof(argv[9]);}

grid grid; grid.generate(min_frequency, max_frequency, resolution);

std::cout << "\n" "Directory location: " << files_location << "\n";
std::cout << "Min frequency: " << min_frequency << "\n";
std::cout << "Max frequency: " << max_frequency << "\n";
std::cout << "Step size: " << grid.fstep << "\n";

const unsigned int no_steps = grid.freq.size();


std::cout << "Number of steps: " << no_steps << "\n";
std::cout << "Required avg/max power: " << min_power << "\n";
std::cout << "Frequency filter range: " << filter_range << "\n";
std::cout << "Amplitude range: " << "[" << min_amplitude << " , " << max_amplitude << "]" << "\n";

//const int max_thread_number = std::thread::hardware_concurrency();

//creates files array
std::vector<std::string> files;
auto directory_iterator = std::filesystem::directory_iterator(argv[2]);
unsigned int file_count = 0;
for (auto& entry : directory_iterator)
{files.push_back(entry.path());++file_count;}

std::cout <<"Number of files in directory: " << file_count << "\n" << std::endl;
// for(unsigned int i=0; i < files.size(); i++) std::cout << files.at(i) << ','; //prints list of files

const std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();

std::thread printThread;
    printThread = std::thread([&]() {
        printProgress(file_count, startTime);
    });

std::filesystem::path path = filesystem::path(files_location).parent_path(); string output_path = path.string() + "/GLS_output.tsv";
ofstream output_file(output_path);
 output_file.close(); //creates and closes output file

auto out = fmt::output_file(output_path, std::ios_base::app);
{out.print("file	frequency	period	amplitude	power\n");}

#pragma omp parallel for
for (unsigned int i = 0; i < file_count; i++) {
    auto [frequency, amplitude, max_power] = periodogram(grid, files[i]);

    if (filter(frequency, max_power, min_power, filter_range, amplitude, min_amplitude, max_amplitude)) {
        #pragma omp critical
        {// Enter critical section to write to the file
            out.print(fmt::format("{:}\t{:.6f}\t{:.4f}\t{:.3f}\t{:.3f}\n",files[i], frequency, 1/frequency, amplitude, max_power));
        };
    }

#pragma omp critical
{filesComputed += 1;}
}



if (string(argv[1]) == "-g") {progressValue.store(static_cast<int>(1000)), timeLeft.store(static_cast<int>(0));}
else {std::cout << "\r" << "Complete" << std::endl;}

printThread.join();

return;}
