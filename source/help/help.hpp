#include <iostream>


void print_help(){

		printf("\n Generalised Lomb-Scargle periodogram for variable stars, version alpha 1.0.4\n");
		printf(" Based on GLS algorithm by Mathias Zechmeister (https://github.com/mzechmeister/GLS).\n");
		printf(" GitHub page of the project: https://github.com/silkskier/glspeaks \n\n");
std::cout << "\n No mode specified." << std::endl;
std::cout << " Usage: glspeaks" << " [OPTION] \n" << std::endl;
std::cout << "\t-g, --gui \t Opens the application in GUI mode. \n" << std::endl;
std::cout << "\t-s, --spectrum \t Writes GLS power spectrum into tsv file." << std::endl;
std::cout << "\t-p, --peaks \t Prints 20 best-fitting periods of brightness change for the specified data." << std::endl;
std::cout << "\t-b, --batch \t Calculates best-fitting periods of brightness change for all the data files in specified directory and write the filtered results into tsv file.\n" << std::endl;
std::cout << "\t-h, --help \t Prints help page (WIP).\n" << std::endl;

}
