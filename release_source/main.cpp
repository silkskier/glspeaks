#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>


#include "batch_cpu/main_b.hpp"
#include "peaks/main_p.hpp"
#include "spectrum/main_s.hpp"
#include "help/help.hpp"




int main(int argc, char *argv[]) {
if (argc < 2) {
		printf("\n Generalised Lomb-Scargle periodogram for variable stars, version alpha 1.0.3\n");
		printf(" Based on GLS algorithm by Mathias Zechmeister (https://github.com/mzechmeister/GLS).\n");
		printf(" GitHub page of the project: https://github.com/silkskier/glspeaks \n\n");
std::cout << "\n No mode specified." << std::endl;
std::cout << " Usage: " << argv[0] << " [OPTION] \n" << std::endl;
std::cout << "\t-g, --gui \t Opens the application in GUI mode. \n" << std::endl;
std::cout << "\t-s, --spectrum \t Writes GLS power spectrum into tsv file." << std::endl;
std::cout << "\t-p, --peaks \t Prints 20 best-fitting periods of brightness change for the specified data." << std::endl;
std::cout << "\t-b, --batch \t Calculates best-fitting periods of brightness change for all the data files in specified directory and write the filtered results into tsv file.\n" << std::endl;
std::cout << "\t-h, --help \t Prints help page (WIP).\n" << std::endl;
return 1;
}

std::string option(argv[1]);

				if (option == "-s" || option == "--spectrum") {

		if(argc < 6){
	std::cout << "\n Usage: " << argv[0] <<" "<< argv[1] << " <Path to input file> <Min frequency> <Max frequency> <Resolution>\n" << std::endl; return 1;}

	if (std::stoi(argv[5]) > 127) {printf(" Error: Step sizes smaller than 2^-127 unsupported due to the limitations of 32-bit floats"); return 1;}
	if (std::stof(argv[4]) > pow(2 ,23 - std::stoi(argv[5]))) {printf("\n Error: Max frequency greater, than 2^(23 - <Resolution>) unsupported due to 32-bit float limitations"); return 1;}


main_spectrum(argc, argv); return 0;}




				 else if (option == "-p" || option == "--peaks") {

	if(argc < 6){
	std::cout << "\n Usage: " << argv[0] <<" "<< argv[1] << " <Path to input file> <Min frequency> <Max frequency> <Resolution>\n" << std::endl; return 1;}

	if (std::stoi(argv[5]) > 127) {printf(" Error: Step sizes smaller than 2^-127 unsupported due to 32-bit float limitations"); return 1;}
	if (std::stof(argv[4]) > pow(2 ,23 - std::stoi(argv[5]))) {printf("\n Error: Max frequency greater, than 2^(23 - <Resolution>) unsupported due to 32-bit float limitations"); return 1;}


main_peaks(argc, argv);return 0;}




				 else if (option == "-b" || option == "--batch") {

	if(argc < 10){
		std::cout << "\n Usage: " << argv[0] << " " << argv[1] << " <Path to catalog with input data files> <Min frequency> <Max frequency> <Resolution> <Max/Avg> <Frequency filter range> <Min amplitude> <Max amplitude>\n" <<std::endl; return 1;}

	if (std::stoi(argv[5]) > 127) {printf(" Error: Step sizes smaller than 2^-127 unsupported due to the limitations of 32-bit floats"); return 1;}
	if (std::stof(argv[4]) > pow(2 ,23 - std::stoi(argv[5]))) {printf("\n Error: Max frequency greater, than 2^(23 - <Resolution>) unsupported due to 32-bit float limitations"); return 1;}



main_batch(argc, argv);return 0;}




				 else if (option == "-h" || option == "--help") {

print_help();

 return 0;}






				 else if (option == "-g" || option == "--gui") {

    FILE* pipe = popen("zenity --list\ --width=800 --height=250\ --title='Mode selection - glspeaks'\ --text='Please select the mode, in which the application will launch.'\ --column='Mode'\ --column='Description'\
     'Batch' 'Writes best frequencies with corresponding powers ratio and amplitudes for each file in directory into .tsv file'\
	 'Peaks' 'Prints list of 20 best-fitting frequencies with corresponding power ratios and amplitudes for a selected file'\
	 'Spectrum' 'Writes corresponding powers to all of the calculated frequencies for the selected file into .tsv file'\
	 'Help' 'Prints short instruction manual for the application'", "r");
    if (!pipe) return 1;


    char buffer[16];
    fgets(buffer, sizeof(buffer), pipe);
    pclose(pipe);


    // Remove the newline character from the string
    buffer[strlen(buffer) - 1] = '\0';

    // Store the result in a string variable and deletes the oryginal output
    std::string mode(buffer);


//    std::cout << "You selected the " << mode <<" mode." << std::endl;

    if (mode == "Batch") {
    char* argv_batch[10]; unsigned int argc_batch = 10;
        std::cout << "success (batch)" << std::endl;
        return 0;
    }



    else if (mode == "Peaks") {
        char* argv_peaks[6]; unsigned int argc_peaks = 6;
        std::cout << "success (peaks)" << std::endl;
        return 0;
    }



    else if (mode == "Spectrum") {

		system("zenity --info --title 'glspeaks (spectrum mode)' --text='Please select file to be used for computation'");

		char* argv_spectrum[6];
		for (int i = 0; i < 6; i++) {argv_spectrum[i] = (char*) malloc(1024);}

		argv_spectrum[0] = argv[0];
		argv_spectrum[1] = argv[1];
		FILE* pipe_file = popen("zenity --file-selection --title 'file selection window - glspeaks (spectrum mode)'", "r");
		if (!pipe_file) return 1;

		char buffer_file[256];
		fgets(buffer_file, sizeof(buffer_file), pipe_file);
		pclose(pipe_file);

		buffer_file[strlen(buffer_file) - 1] = '\0';

		strcpy(argv_spectrum[2], buffer_file);
		std::cout << argv_spectrum[2] <<std::endl; //prints path to file saved for computation



		FILE* pipe_data = popen("zenity --forms --title='data input - glspeaks' --text='Please input values to be used for calculation' \
		--add-entry='min frequency (1/d)' --add-entry='max frequency (1/d)' --add-entry='resolution'", "r");
		if (!pipe_data) return 1;


		char buffer_data[256];
		fgets(buffer_data, sizeof(buffer_data), pipe_data);
		pclose(pipe_data);

		// Remove the newline character from the string
		buffer_data[strlen(buffer_data) - 1] = '\0';


		// Parse the input string to extract the values
		char argv3[8], argv4[8], argv5[8];
		sscanf(buffer_data, "%7[^|]|%7[^|]|%7s", argv3, argv4, argv5);
		strcpy(argv_spectrum[3], argv3), strcpy(argv_spectrum[4], argv4), strcpy(argv_spectrum[5], argv5);

		//std::cout << "You entered: " << argv_spectrum[3] << " " << argv_spectrum[4] << " " << argv_spectrum[5] << " " << std::endl;

		main_spectrum(6, argv_spectrum);



        //std::cout << "success (spectrum)" << std::endl;
        return 0;
    }



    else if (mode == "Help") {

    std::stringstream buffer;
    std::streambuf *coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(buffer.rdbuf());

	print_help();

    std::cout.rdbuf(coutbuf);

    std::string output = buffer.str();

    std::FILE *pipe = popen("zenity --text-info \ --width=1080 --height=250 \ --title 'help window - glspeaks' ", "w");
    fwrite(output.c_str(), output.size(), 1, pipe);
    pclose(pipe);

    return 0;


		/*
    freopen("zenity --info --text='Output of the function:' |", "w", stdout);

    std::cout << " Help page not available in release alpha 1.0.\n Planned as addition to full 1.0 release." <<std::endl;

    fclose(stdout);
    return 0;

    */
    }



    else {
        std::cerr << "Error: no mode selected." << std::endl;
        return 1;
   }}



/*
    FILE* pipe = popen("zenity --forms --add-entry=godziny --add-entry=minuty --add-entry=sekundy --text='Podaj kąt (w godzinach)'", "r");
    if (!pipe) return 1;

    char buffer[1024];
    fgets(buffer, sizeof(buffer), pipe);
    pclose(pipe);

    // Remove the newline character from the string
    buffer[strlen(buffer) - 1] = '\0';

    // Parse the input string to extract the values
    int hour, minute, second;
    sscanf(buffer, "%d|%d|%d", &hour, &minute, &second);

    std::cout << "You entered: " << hour << " hours, " << minute << " minutes, and " << second << " seconds." << std::endl;
 return 0;}
*/





 else {
std::cerr << "Error: Invalid mode selected." << std::endl;
return 1;
}}
