#include "qt.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("\n Generalised Lomb-Scargle periodogram for variable stars, version alpha 1.0.4\n");
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

    switch (option[0]) {
        case '-':
            switch (option[1]) {
                case 's':
                    if (option == "-s" || option == "--spectrum") {
                        if (argc < 3) {
                            std::cout << "\n Usage: " << argv[0] << " " << argv[1] << " <Path to input file> <Min frequency> <Max frequency> <Resolution>\n" << std::endl;
                            return 1;
                        }
                        if (argc > 5) {
                            if (std::stoi(argv[5]) > 127) {
                                printf(" Error: Step sizes smaller than 2^-127 unsupported due to 32-bit float limitations");
                                return 1;
                            }
                        }
                        if (argc > 5) {
                            if (std::stof(argv[4]) > pow(2, 23 - std::stoi(argv[5]))) {
                                std::cout << "Error - glspeaks' --text='Max frequency greater, than 2^(23 - [Resolution]) unsupported due to 32-bit float limitations" << std::endl;
                                return 1;
                            }
                        }
                        if (argc > 4) {
                            if (std::stof(argv[4]) > pow(2, 23 - 12)) {
                                std::cout << "Error - glspeaks' --text='Max frequency greater, than 2^(23 - [Resolution]) unsupported due to 32-bit float limitations" << std::endl;
                                return 1;
                            }
                        }
                        main_spectrum(argc, argv);
                        return 0;
                    }
                    break;
                case 'p':
                    if (option == "-p" || option == "--peaks") {
                        if (argc < 3) {
                            std::cout << "\n Usage: " << argv[0] << " " << argv[1] << " <Path to input file> <Min frequency> <Max frequency> <Resolution>\n" << std::endl;
                            return 1;
                        }
                        if (argc > 5) {
                            if (std::stoi(argv[5]) > 127) {
                                printf(" Error: Step sizes smaller than 2^-127 unsupported due to 32-bit float limitations");
                                return 1;
                            }
                        }
                        if (argc > 5) {
                            if (std::stof(argv[4]) > pow(2, 23 - std::stoi(argv[5]))) {
                                std::cout << "Error - glspeaks' --text='Max frequency greater, than 2^(23 - [Resolution]) unsupported due to 32-bit float limitations'" << std::endl;
                                return 1;
                            }
                        }
                        if (argc > 4) {
                            if (std::stof(argv[4]) > pow(2, 23 - 12)) {
                                std::cout << "Error - glspeaks' --text='Max frequency greater, than 2^(23 - [Resolution]) unsupported due to 32-bit float limitations'" << std::endl;
                                return 1;
                            }
                        }
                        main_peaks(argc, argv);
                        return 0;
                    }
                    break;
                case 'b':
                    if (option == "-b" || option == "--batch") {
                        if (argc < 3) {
                            std::cout << "\n Usage: " << argv[0] << " " << argv[1] << " <Path to catalog with input data files> <Min frequency> <Max frequency> <Resolution> <Max/Avg> <Frequency filter range> <Min amplitude> <Max amplitude>\n" << std::endl;
                            return 1;
                        }
                        if (argc > 5) {
                            if (std::stoi(argv[5]) > 127) {
                                printf(" Error: Step sizes smaller than 2^-127 unsupported due to the limitations of 32-bit floats");
                                return 1;
                            }
                        }
                        if (argc > 5) {
                            if (std::stof(argv[4]) > pow(2, 23 - std::stoi(argv[5]))) {
                                std::cout << "Error - glspeaks' --text='Max frequency greater, than 2^(23 - [Resolution]) unsupported due to 32-bit float limitations'" << std::endl;
                                return 1;
                            }
                        }
                        if (argc > 5) {
                            if (std::stof(argv[4]) > pow(2, 23 - std::stoi(argv[5]))) {
                                std::cout << "Error - glspeaks' --text='Max frequency greater, than 2^(23 - [Resolution]) unsupported due to 32-bit float limitations'" << std::endl;
                                return 1;
                            }
                        }
                        if (argc > 4) {
                            if (std::stof(argv[4]) > pow(2, 23 - 12)) {
                                std::cout << "Error - glspeaks' --text='Max frequency greater, than 2^(23 - [Resolution]) unsupported due to 32-bit float limitations'" << std::endl;
                                return 1;
                            }
                        }
                        main_batch(argc, argv);
                        return 0;
                    }
                    break;
                case 'h':
                    if (option == "-h" || option == "--help") {
                        print_help();
                        return 0;
                    }
                    break;
                case 'g':
                    if (option == "-g" || option == "--gui") {
                        qt(argc, argv);
                    }
                    break;
                case 'S':
                    if (option == "-S" || option == "--serialize") {
                        main_serialization(argc, argv);
                    }
                    break;
                                case 'D':
                    if (option == "-D" || option == "--deserialize") {
                        main_serialization(argc, argv);
                    }
                    break;
                default:
                    break;
            }
            break;
        default:
            std::cerr << "Error: Invalid mode selected." << std::endl;
            return 1;
    }
}
