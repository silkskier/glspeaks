#include <iostream>
#include <string>
#include <filesystem>
#include <cstdlib>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <math.h>
#include <vector>

// using namespace std;

int main(int argc, char *argv[])
{

//Message to print if there is not enough given arguments
 if(argc < 5){
     printf("\n FNPEAKS_batch for Linux, version of 2022\n");
     printf(" by W. Hebisch, Z. Kolaczkowski and G. Kopacki.\n");
     printf(" Computing of amplitude spectrum for time series data.\n\n");
     printf(" Usage: fnpeaks <Path to catalog with data files> <Min frequency> <Max frequency> <Resolution>\n\n");
     return 0;

}else{

//defines variables used for calculations
const std::string files_location = argv[1];
const float min_frequency = std::stof(argv[2]);
const float max_frequency = std::stof(argv[3]);
const float step_size = pow(0.5,std::stoi(argv[4]));
std::cout << "Directory location: " << files_location << "\n";
std::cout << "Min frequency: " << min_frequency << "\n";
std::cout << "Max frequency: " << max_frequency << "\n";
std::cout << "Step size: " << step_size << "\n";

//creates frequency array
const int no_steps = (max_frequency - min_frequency)/step_size + 1;
std::cout << "Number of steps: " << no_steps << "\n";
int step = 0;
std::vector <float> steps;
while(step < no_steps){steps.push_back(min_frequency + step*step_size);step++;}
//      for(size_t i=0; i <steps.size(); i++) printf("%f, ", steps[i]); // prints frequency array

//creates files array

return 0;}
}
