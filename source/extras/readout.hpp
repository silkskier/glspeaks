#ifndef READOUT_HPP
#define READOUT_HPP

#include <ostream>

#include <fmt/ostream.h>
#include <fmt/format.h>
#include <fmt/core.h>
#include <fmt/format-inl.h>
#include <fmt/os.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <unordered_map>
#include <mutex>

#include "../../include/fast_float.h"

#include "../../include/yas/mem_streams.hpp"
#include "../../include/yas/binary_iarchive.hpp"
#include "../../include/yas/binary_oarchive.hpp"
#include "../../include/yas/std_types.hpp"
#include "../../include/yas/file_streams.hpp"




struct star {
    std::string id;
    std::vector<double> x;
    std::vector<float> y;
    std::vector<float> dy;

    inline void save(const std::string& out_path) {
    std::string file_path = out_path + "/" + id + ".dat";

    std::ofstream ofs(file_path);
    ofs.close();

    auto out = fmt::output_file(file_path);

    for (unsigned int i = 0; i < x.size(); i++){
        out.print(fmt::format("{:.5f} {:.3f} {:.3f}\n", x[i], y[i], dy[i]));
    }}

    inline void read(const std::string& in_file) {
        std::ifstream input_file(in_file);

        if (input_file) {
            static const auto BUFFER_SIZE = 16 * 1024;
            char buf[BUFFER_SIZE + 1];
            uintmax_t lines = 0;
            std::vector<char> dataBuffer; // To store the loaded data

            while (input_file) {
                input_file.read(buf, BUFFER_SIZE);
                size_t bytes_read = input_file.gcount();

                if (bytes_read == 0) {
                    break;
                }

                dataBuffer.insert(dataBuffer.end(), buf, buf + bytes_read); // Append data to buffer

                for (size_t i = 0; i < bytes_read; ++i) {
                    if (buf[i] == '\n') {
                        ++lines;
                    }
                }
            }

            // Set up iterators for parsing from dataBuffer
            char const* it = dataBuffer.data();
            char const* end = it + dataBuffer.size();

            double tempX;
            float tempY;
            float tempDY;

            while (it < end) {
                auto temp = fast_float::from_chars(it, end, tempX);

                if (temp.ec != std::errc()) {
                    std::cout << "Parsing failed" << std::endl;
                    break;
                }
                if(temp.ptr[0] != ' ' && temp.ptr[0] != '\n' ) {
                    std::cout << "Unexpected delimiter" << std::endl;
                }


                temp = fast_float::from_chars(temp.ptr + 1, end, tempY);

                if (temp.ec != std::errc()) {
                    std::cout << "Parsing failed" << std::endl;
                    break;
                }

                if(temp.ptr[0] != ' ' && temp.ptr[0] != '\n' ) {
                    std::cout << "Unexpected delimiter" << std::endl;
                }


                temp = fast_float::from_chars(temp.ptr + 1, end, tempDY);

                if (temp.ec != std::errc()) {
                    std::cout << "Parsing failed" << std::endl;
                    break;
                }

                if(temp.ptr[0] != ' ' && temp.ptr[0] != '\n' ) {
                    std::cout << "Unexpected delimiter" << std::endl;
                }


                it = temp.ptr + 1;

                x.push_back(tempX);
                y.push_back(tempY);
                dy.push_back(tempDY);
            }

            id = in_file.substr(in_file.find_last_of('/') + 1, in_file.find_last_of('.') - in_file.find_last_of('/') - 1); // Add star id to the struct
        } else {
            std::cout << "File cannot be opened" << std::endl;
        }

        input_file.close();
    }

    template <typename Archive>
    inline void serialize(Archive& ar) {
    ar & id & x & y & dy;
    }
};





struct photometry {
    std::vector<star> stars;
    std::unordered_map<std::string, int> id;
    std::mutex mutex;

    template <typename Archive>
    void serialize(Archive& ar) {
        ar & stars & id;
    }

    void save_dat(const std::string& outdir) {

    if (!std::filesystem::exists(outdir)) {
        if (!std::filesystem::create_directories(outdir)) {
            std::cerr << "Error creating directory: " << outdir << std::endl;
            return;
        }
    }

        #pragma omp parallel for
        for (unsigned int i = 0; i < stars.size(); i++) {
            stars[i].save(outdir);
    }}

    void save(const std::string& filename) {
        const char* filename_cstr = filename.c_str();

        yas::file_ostream fos(filename_cstr);
        yas::binary_oarchive<yas::file_ostream> oa(fos);

        // Serialize and save the photometry struct
        oa & *this;
    }

    inline void add(const star &star) {
    std::lock_guard<std::mutex> lock(mutex);

    stars.push_back(star);
    id[star.id] = stars.size() - 1;
    }

    // Deserialize and load the photometry struct from file
    void load(const std::string& file) {
        const char* file_cstr = file.c_str();

        yas::file_istream fis(file_cstr);
        yas::binary_iarchive<yas::file_istream> ia(fis);

        ia & *this;
    }

    void load_dat(std::string in_dir){
        std::vector<std::filesystem::path> files;
        for (auto& entry : std::filesystem::directory_iterator(in_dir)) {
            if (entry.is_regular_file() && entry.path().extension() == ".dat") {
                files.push_back(entry.path());
            }
        }

        #pragma omp parallel for
        for (unsigned int i = 0; i < files.size(); i++) {
            star data;
            data.read(files[i]);
            add(data);}
    }
};


#endif
