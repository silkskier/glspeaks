#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <regex>

#include <unistd.h>


#include "batch_cpu/main_b.hpp"
#include "peaks/main_p.hpp"
#include "spectrum/main_s.hpp"
#include "help/help.hpp"




int main(int argc, char *argv[]) {

std::string option = "--help";
if (argc > 1) {option = (argv[1]);}


				if (option == "-s" || option == "--spectrum") {

		if(argc < 5){
	std::cout << "\n Usage: " << argv[0] <<" "<< argv[1] << " <Path to input file> <Min frequency> <Max frequency> <Resolution>\n" << std::endl; return 1;}

	if (argc > 5) {if (std::stoi(argv[5]) > 127) {printf(" Error: Step sizes smaller than 2^-127 unsupported due to 32-bit float limitations"); return 1;}}

	if (argc > 5) {if (std::stof(argv[4]) > pow(2 ,23 - std::stoi(argv[5]))){system("zenity --error --title 'Error - glspeaks' --text='Max frequency greater, than 2^(23 - [Resolution]) unsupported due to 32-bit float limitations'"); return 1;}}
	else {if (std::stof(argv[4]) > pow(2 ,23 - 12)){system("zenity --error --title 'Error - glspeaks' --text='Max frequency greater, than 2^(23 - [Resolution]) unsupported due to 32-bit float limitations'"); return 1;}}



main_spectrum(argc, argv); return 0;}




				 else if (option == "-p" || option == "--peaks") {

	if(argc < 5){
	std::cout << "\n Usage: " << argv[0] <<" "<< argv[1] << " <Path to input file> <Min frequency> <Max frequency> <Resolution>\n" << std::endl; return 1;}

	if (argc > 5) {if (std::stoi(argv[5]) > 127) {printf(" Error: Step sizes smaller than 2^-127 unsupported due to 32-bit float limitations"); return 1;}}

	if (argc > 5) {if (std::stof(argv[4]) > pow(2 ,23 - std::stoi(argv[5]))){system("zenity --error --title 'Error - glspeaks' --text='Max frequency greater, than 2^(23 - [Resolution]) unsupported due to 32-bit float limitations'"); return 1;}}
	else {if (std::stof(argv[4]) > pow(2 ,23 - 12)){system("zenity --error --title 'Error - glspeaks' --text='Max frequency greater, than 2^(23 - [Resolution]) unsupported due to 32-bit float limitations'"); return 1;}}



main_peaks(argc, argv);return 0;}




				 else if (option == "-b" || option == "--batch") {

	if(argc < 5){
		std::cout << "\n Usage: " << argv[0] << " " << argv[1] << " <Path to catalog with input data files> <Min frequency> <Max frequency> <Resolution> <Max/Avg> <Frequency filter range> <Min amplitude> <Max amplitude>\n" <<std::endl; return 1;}

	if (argc > 5) {if (std::stoi(argv[5]) > 127) {printf(" Error: Step sizes smaller than 2^-127 unsupported due to the limitations of 32-bit floats"); return 1;}}

	if (argc > 5) {if (std::stof(argv[4]) > pow(2 ,23 - std::stoi(argv[5]))){system("zenity --error --title 'Error - glspeaks' --text='Max frequency greater, than 2^(23 - [Resolution]) unsupported due to 32-bit float limitations'"); return 1;}}

	if (argc > 5) {if (std::stof(argv[4]) > pow(2 ,23 - std::stoi(argv[5]))){system("zenity --error --title 'Error - glspeaks' --text='Max frequency greater, than 2^(23 - [Resolution]) unsupported due to 32-bit float limitations'"); return 1;}}
	else {if (std::stof(argv[4]) > pow(2 ,23 - 12)){system("zenity --error --title 'Error - glspeaks' --text='Max frequency greater, than 2^(23 - [Resolution]) unsupported due to 32-bit float limitations'"); return 1;}}




main_batch(argc, argv);return 0;}




				 else if (option == "-h" || option == "--help") {

print_help();

 return 0;}






				 else if (option == "-g" || option == "--gui") {

    FILE* pipe = popen("zenity --list\ --width=800 --height=250\ --title='Mode selection - glspeaks'\
     --text='Please select the mode, in which the application will launch.'\ --column='Mode'\ --column='Description'\
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
    char* argv_batch[10]; unsigned int argc_batch = 3;

		system("zenity --info --title 'glspeaks (batch mode)' --text='Please select catalog with data files to be used for computation'");

		argv_batch[0] = (char*) malloc(256);
		argv_batch[1] = (char*) malloc(16);
		argv_batch[2] = (char*) malloc(256);
		argv_batch[3] = (char*) malloc(16);
		argv_batch[4] = (char*) malloc(16);
		argv_batch[5] = (char*) malloc(16);
		argv_batch[6] = (char*) malloc(16);
		argv_batch[7] = (char*) malloc(16);
		argv_batch[8] = (char*) malloc(16);
		argv_batch[9] = (char*) malloc(16);


		argv_batch[0] = argv[0];
		argv_batch[1] = argv[1];

		FILE* pipe_dir = popen("zenity --file-selection --directory --title 'file selection window - glspeaks (batch mode)'", "r");
		if (!pipe_dir) return 1;

		char buffer_dir[256];
		fgets(buffer_dir, sizeof(buffer_dir), pipe_dir);
		pclose(pipe_dir);

		buffer_dir[strlen(buffer_dir) - 1] = '\0';

		strcpy(argv_batch[2], buffer_dir);

		FILE* pipe_data = popen("zenity --forms --title='data input - glspeaks (batch mode)'\
		 --text='	Please input values to be used for calculation. \n\n	Warning: The parser reads the input only to the first empty field. \n	To pass an argument to function all arguments preciding\
		 \n	argument to be passed to application must be specified. \n\n	If value of the argument or any of the preceding ones\n	is not specified it will take a default value.' \
		--add-entry='min frequency [1/d]' --add-entry='max frequency [1/d]' --add-entry='resolution (default = 12)' --add-entry='required max/avg power ratio (default = 1)'\
		 --add-entry='frequency filter range (default = 0.02)' --add-entry='min amplitude (default = 0)' --add-entry='max amplitude (default = 8)'", "r");
		if (!pipe_data) return 1;


		char buffer_data[256];
		fgets(buffer_data, sizeof(buffer_data), pipe_data);
		pclose(pipe_data);

		// Remove the newline character from the string
		buffer_data[strlen(buffer_data) - 1] = '\0';


		// Parse the input string to extract the values
		char argv3[16], argv4[16], argv5[16], argv6[16], argv7[16], argv8[16], argv9[16];
		sscanf(buffer_data, "%15[^|]|%15[^|]|%15[^|]|%15[^|]|%15[^|]|%15[^|]|%15s", argv3, argv4, argv5, argv6, argv7, argv8, argv9);
		strcpy(argv_batch[3], argv3), strcpy(argv_batch[4], argv4), strcpy(argv_batch[5], argv5), strcpy(argv_batch[6], argv6),
		strcpy(argv_batch[7], argv7), strcpy(argv_batch[8], argv8), strcpy(argv_batch[9], argv9);

		for (int i = 3; i < 10; i++) {if (isdigit(argv_batch[i][0])) {argc_batch++;}}

		if (argc_batch < 5) {
		system("zenity --error --title 'Error - glspeaks' --text='Min or max frequency not specified.'");}

//		std::cout<<"\n argc_batch = " << argc_batch <<std::endl;


		if (argc_batch > 5) {if (std::stoi(argv_batch[5]) > 127)
		{system("zenity --error --title 'Error - glspeaks' --text='Step sizes smaller than 2^-127 unsupported due to 32-bit float limitations'"); return 1;}}

		if (argc_batch > 5) {if (std::stof(argv_batch[4]) > pow(2 ,23 - std::stoi(argv_batch[5]))){system("zenity --error --title 'Error - glspeaks' --text='Max frequency greater, than 2^(23 - [Resolution]) unsupported due to 32-bit float limitations'"); return 1;}}
		else {if (std::stof(argv_batch[4]) > pow(2 ,23 - 12)){system("zenity --error --title 'Error - glspeaks' --text='Max frequency greater, than 2^(23 - [Resolution]) unsupported due to 32-bit float limitations'"); return 1;}}


		//Create Zenity progressbar for calculations
		FILE* pipe = popen("bash -c \"zenity --progress --width=640 --title='glspeaks (batch mode)' --text='Calculations in progress.' --percentage=0 --auto-close --time-remaining --auto-kill\"", "w");
		// Redirect stdout to the Zenity pipe
		fflush(stdout);
		int pipe_fd = fileno(pipe);
		dup2(pipe_fd, STDOUT_FILENO);

		//Start the calculations main_batch function
		main_batch(argc_batch, argv_batch);

		// Close the Zenity pipe
		pclose(pipe);

		system("zenity --info --title 'glspeaks (batch mode)' --text='Calculations complete. Output data saved to GLS_output.tsv file in the parent directory of selected input data directory.'");

        return 0;
    }




    else if (mode == "Peaks") {

		system("zenity --info --title 'glspeaks (peaks mode)' --text='Please select file to be used for computation'");

		char* argv_peaks[6]; int argc_peaks = 3;

		argv_peaks[0] = (char*) malloc(256);
		argv_peaks[1] = (char*) malloc(16);
		argv_peaks[2] = (char*) malloc(256);
		argv_peaks[3] = (char*) malloc(16);
		argv_peaks[4] = (char*) malloc(16);
		argv_peaks[5] = (char*) malloc(16);

		argv_peaks[0] = argv[0];
		argv_peaks[1] = argv[1];
		FILE* pipe_file = popen("zenity --file-selection --title 'file selection window - glspeaks (peaks mode)'", "r");
		if (!pipe_file) return 1;

		char buffer_file[256];
		fgets(buffer_file, sizeof(buffer_file), pipe_file);
		pclose(pipe_file);

		buffer_file[strlen(buffer_file) - 1] = '\0';

		strcpy(argv_peaks[2], buffer_file);

		FILE* pipe_data = popen("zenity --forms --title='data input - glspeaks (peaks mode)' --text='Please input values to be used for calculation' \
		--add-entry='min frequency (1/d)' --add-entry='max frequency (1/d)' --add-entry='resolution (default = 12)'", "r");
		if (!pipe_data) return 1;


		char buffer_data[256];
		fgets(buffer_data, sizeof(buffer_data), pipe_data);
		pclose(pipe_data);

		// Remove the newline character from the string
		buffer_data[strlen(buffer_data) - 1] = '\0';


		// Parse the input string to extract the values
		char argv3[16], argv4[16], argv5[16];
		sscanf(buffer_data, "%15[^|]|%15[^|]|%15s", argv3, argv4, argv5);
		strcpy(argv_peaks[3], argv3), strcpy(argv_peaks[4], argv4), strcpy(argv_peaks[5], argv5);

		for (int i = 3; i < 5; i++) {if (isdigit(argv_peaks[i][0])) {argc_peaks++;}}

		if (argc_peaks < 5) {
		system("zenity --error --title 'Error - glspeaks' --text='Min or max frequency not specified.'");}



		if (argc_peaks > 5) {if (std::stoi(argv_peaks[5]) > 127)
		{system("zenity --error --title 'Error - glspeaks' --text='Step sizes smaller than 2^-127 unsupported due to 32-bit float limitations'"); return 1;}}

		if (argc_peaks > 5) {if (std::stof(argv_peaks[4]) > pow(2 ,23 - std::stoi(argv_peaks[5]))){system("zenity --error --title 'Error - glspeaks' --text='Max frequency greater, than 2^(23 - [Resolution]) unsupported due to 32-bit float limitations'"); return 1;}}
		else {if (std::stof(argv_peaks[4]) > pow(2 ,23 - 12)){system("zenity --error --title 'Error - glspeaks' --text='Max frequency greater, than 2^(23 - [Resolution]) unsupported due to 32-bit float limitations'"); return 1;}}





std::stringstream output_buffer;
std::streambuf *coutbuf = std::cout.rdbuf();
std::cout.rdbuf(output_buffer.rdbuf());

main_peaks(argc_peaks, argv_peaks);

std::cout.rdbuf(coutbuf);

std::string output = output_buffer.str();
if (output.length() > 0 && output[0] == '\n') {output.erase(0, 1);}

// Replace double tabs with single tabs
while (output.find("\t\t") != std::string::npos) {output = std::regex_replace(output, std::regex("\t\t"), "\t");}

// Replace tabs with spaces
size_t start_position = 0;

std::string tab_replacement = "                                                "; // 49 spaces
int tab_width = 48;
size_t line_start = 0;
size_t pos = 0;
while ((pos = output.find("\n", pos)) != std::string::npos) {
  size_t line_end = pos;
  pos++;
  size_t current_pos = line_start;

while (current_pos < line_end) {
	if (line_start > start_position){start_position = line_start;}
    size_t tab_pos = output.find("\t", current_pos);
    if (tab_pos == std::string::npos || tab_pos >= line_end) {break;}

    size_t spaces_to_add = tab_width - (3*(tab_pos - start_position) % tab_width);
    output.replace(tab_pos, 1, tab_replacement.substr(0, spaces_to_add));
	//output.replace(tab_pos + spaces_to_add - 1, 1, "\t"); //somehow breaks Zenity.
    current_pos = tab_pos + spaces_to_add;
    start_position = current_pos;
  }
  line_start = pos;
}

std::FILE *pipe = popen("zenity --text-info --width=492 --height=536 --title 'results - glspeaks (peaks mode)'", "w");
fwrite(output.c_str(), output.size() - 1, 1, pipe); //remove "\n" from the output string
pclose(pipe);


        return 0;
    }




    else if (mode == "Spectrum") {

		system("zenity --info --title 'glspeaks (spectrum mode)' --text='Please select file to be used for computation'");

		char* argv_spectrum[6]; int argc_spectrum = 3;

		argv_spectrum[0] = (char*) malloc(256);
		argv_spectrum[1] = (char*) malloc(16);
		argv_spectrum[2] = (char*) malloc(256);
		argv_spectrum[3] = (char*) malloc(16);
		argv_spectrum[4] = (char*) malloc(16);
		argv_spectrum[5] = (char*) malloc(16);

		argv_spectrum[0] = argv[0];
		argv_spectrum[1] = argv[1];
		FILE* pipe_file = popen("zenity --file-selection --title 'file selection window - glspeaks (spectrum mode)'", "r");
		if (!pipe_file) return 1;

		char buffer_file[256];
		fgets(buffer_file, sizeof(buffer_file), pipe_file);
		pclose(pipe_file);

		buffer_file[strlen(buffer_file) - 1] = '\0';

		strcpy(argv_spectrum[2], buffer_file);
		//std::cout << argv_spectrum[2] <<std::endl; //prints path to file saved for computation


		FILE* pipe_data = popen("zenity --forms --title='data input - glspeaks' --text='Please input values to be used for calculation' \
		--add-entry='min frequency (1/d)' --add-entry='max frequency (1/d)' --add-entry='resolution (default = 12)'", "r");
		if (!pipe_data) return 1;


		char buffer_data[256];
		fgets(buffer_data, sizeof(buffer_data), pipe_data);
		pclose(pipe_data);

		// Remove the newline character from the string
		buffer_data[strlen(buffer_data) - 1] = '\0';


		// Parse the input string to extract the values
		char argv3[16], argv4[16], argv5[16];
		sscanf(buffer_data, "%15[^|]|%15[^|]|%15s", argv3, argv4, argv5);
		strcpy(argv_spectrum[3], argv3), strcpy(argv_spectrum[4], argv4), strcpy(argv_spectrum[5], argv5);


		for (int i = 3; i < 6; i++) {if (isdigit(argv_spectrum[i][0])) {argc_spectrum++;}}

		if (argc_spectrum < 5) {
		system("zenity --error --title 'Error - glspeaks' --text='Min or max frequency not specified.'");}

		//std::cout << "You entered: " << argv_spectrum[3] << " " << argv_spectrum[4] << " " << argv_spectrum[5] << " " << std::endl;

		if (argc_spectrum > 5) {if (std::stoi(argv_spectrum[5]) > 127)
		{system("zenity --error --title 'Error - glspeaks' --text='Step sizes smaller than 2^-127 unsupported due to 32-bit float limitations'"); return 1;}}

		if (argc_spectrum > 5) {if (std::stof(argv_spectrum[4]) > pow(2 ,23 - std::stoi(argv_spectrum[5]))){system("zenity --error --title 'Error - glspeaks' --text='Max frequency greater, than 2^(23 - [Resolution]) unsupported due to 32-bit float limitations'"); return 1;}}
		else {if (std::stof(argv_spectrum[4]) > pow(2 ,23 - 12)){system("zenity --error --title 'Error - glspeaks' --text='Max frequency greater, than 2^(23 - [Resolution]) unsupported due to 32-bit float limitations'"); return 1;}}


		main_spectrum(argc_spectrum, argv_spectrum);

		system("zenity --info --title 'glspeaks (spectrum mode)' --text='Spectrum saved to .tsv file in the parent directory of selected input file.'");


        return 0;
    }



    else if (argc < 2 || mode == "Help") {

    std::stringstream output_buffer;
    std::streambuf *coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(output_buffer.rdbuf());

	print_help();

    std::cout.rdbuf(coutbuf);

    std::string output = output_buffer.str();

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
    }}


 else {
std::cerr << "Error: Invalid mode selected." << std::endl;
return 1;
}}
