#include <iostream>


void print_help(){

	printf("\n");
std::cout<< " Generalised Lomb-Scargle periodogram for variable stars, version alpha 1.0.4\n"<<" Based on GLS algorithm by Mathias Zechmeister (https://github.com/mzechmeister/GLS).\n" <<" GitHub page of the project: https://github.com/silkskier/glspeaks \n" <<std::endl;


std::cout << " Usage: glspeaks" << " [OPTION] \n" << std::endl;
std::cout << "\t-g, --gui \t\t Opens the application in GUI mode. \n" << std::endl;
std::cout << "\t-s, --spectrum \t Writes GLS power spectrum into tsv file." << std::endl;
std::cout << "\t-p, --peaks \t Prints 20 best-fitting periods of brightness change for the specified data." << std::endl;
std::cout << "\t-b, --batch \t Calculates best-fitting periods of brightness change for all the data files \n\t in specified directory and write the filtered results into tsv file.\n" << std::endl;
std::cout << "\t-h, --help \t Prints help page (WIP).\n" << std::endl;

}
