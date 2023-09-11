#include "../utils/readout.hpp"

void main_serialization(int argc, char *argv[]){

    std::string mode = std::string(argv[1]);

    if (mode == "-S" || mode == "--serialize"){

    if (argc < 3){std::cout << "No source directory specified" << std::endl; return;}
    std::cout << "Source directory:\t" << argv[2] << std::endl;
    std::string output_path = std::string(argv[2]) + ".phot";
    std::cout << "Output file:\t" << output_path << std::endl;

    /*
    //create files array, to be used for mapping directories in the future
    std::vector<filesystem::path> files; uint file_count = 0;
    for (auto& entry : std::filesystem::directory_iterator(argv[2])) {
        if (entry.is_regular_file() && entry.path().extension() == ".dat") {
            files.push_back(entry.path());
            ++file_count;
        }
    }*/

    photometry photometry;
    photometry.load_dat(argv[2]);
    photometry.save(output_path);

    }

    else if (mode == "-D" || mode == "--deserialize"){
    if (argc < 3){std::cout << "No source file specified" << std::endl; return;}

    std::string output_path = std::string(argv[2]).substr(0, std::string(argv[2]).find_last_of('.'));

    std::cout << "Path without extension: " << output_path << std::endl;

    photometry photometry;
    photometry.load(argv[2]);
    photometry.save_dat(output_path);
    }


return;}
