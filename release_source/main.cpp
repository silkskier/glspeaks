#include <iostream>
#include <string>

#include "main_b.hpp"
#include "main_p.hpp"
#include "main_s.hpp"





int main(int argc, char *argv[]) {
if (argc < 2) {
std::cout << "Error: Please specify an option." << std::endl;
std::cout << "Usage: " << argv[0] << " [OPTION]" << std::endl;
std::cout << " -b, --batch Run main_batch function." << std::endl;
std::cout << " -p, --peaks Run main_peaks function." << std::endl;
std::cout << " -s, --spectrum Run main_spectrum function." << std::endl;
return 1;
}

std::string option(argv[1]);

				if (option == "-s" || option == "--spectrum") {

		if(argc < 6){
     printf("\n Generalized Lomb-Scargle periodogram for variable stars, version of 2022\n");
     printf(" based on C version of GLS by Mathias Zechmeister (https://github.com/mzechmeister/GLS).\n");
     printf(" Computing of amplitude spectrum for time series data.\n\n");
     printf(" Usage: GLS <Path to file> <Min frequency> <Max frequency> <Resolution>\n\n"); return 1;}

if (std::stoi(argv[5]) > 23) {printf(" Error: Step sizes smaller than 2^-23 unsupported due to limitations of 32-bit floats"); return 1;}



main_spectrum(argc, argv); return 0;}











				 else if (option == "-p" || option == "--peaks") {

	if(argc < 6){
     printf("\n Generalized Lomb-Scargle periodogram for variable stars, version of 2022\n");
     printf(" based on C version of GLS by Mathias Zechmeister (https://github.com/mzechmeister/GLS).\n");
     printf(" Computing of amplitude spectrum for time series data.\n\n");
     printf(" Usage: GLS <Path to file> <Min frequency> <Max frequency> <Resolution>\n\n"); return 1;}

if (std::stoi(argv[5]) > 23) {printf(" Error: Step sizes smaller than 2^-23 unsupported due to limitations of 32-bit floats"); return 1;}



main_peaks(argc, argv);return 0;}







				 else if (option == "-b" || option == "--batch") {

	if(argc < 10){
		printf("\n Batch GLS period finder for variable stars, version 1.0\n");
		printf(" based on GLS algorithm by Mathias Zechmeister (https://github.com/mzechmeister/GLS)\n\n");
		std::cout << " Usage: " << argv[0] << " " << argv[1] << " <Path to catalog with data> <Min frequency> <Max frequency> <Resolution> <Max/Avg> <Filter range> <Min amplitude> <Max amplitude>\n" <<std::endl; return 1;}

if (std::stoi(argv[5]) > 23) {printf(" Error: Step sizes smaller than 2^-23 unsupported due to limitations of 32-bit floats"); return 1;}
if(stof(argv[3]) <= 0){printf("\n Error; Starting frequency must be greater, than 0\n"); return 1;}

main_batch(argc, argv);return 0;}

 else {
std::cout << "Error: Invalid mode selected." << std::endl;
return 1;
}}
