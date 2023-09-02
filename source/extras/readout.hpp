#ifndef READOUT_HPP
#define READOUT_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <unordered_map>

#include <boost/spirit/include/qi.hpp>

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

    template <typename Archive>
    inline void serialize(Archive& ar) {
    ar & id & x & y & dy;
    }
};

struct photometry {
    std::vector<star> stars;
    std::unordered_map<std::string, int> id;

    template <typename Archive>
    void serialize(Archive& ar) {
        ar & stars & id;
    }

    void save(const std::string& filename) {
        const char* filename_cstr = filename.c_str();

        yas::file_ostream fos(filename_cstr);
        yas::binary_oarchive<yas::file_ostream> oa(fos);

        // Serialize and save the photometry struct
        oa & *this;
    }
};


star read_dat(const std::string& in_file) {
    star data;
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


    bool success = boost::spirit::qi::phrase_parse(it, end, boost::spirit::qi::double_ >> boost::spirit::qi::float_ >> boost::spirit::qi::float_, boost::spirit::qi::space, tempX, tempY, tempDY);

    if (success) {
        data.x.push_back(tempX);
        data.y.push_back(tempY);
        data.dy.push_back(tempDY);
    } else {
        std::cout << "Parsing failed" << std::endl;
        break;
    }
}

        //std::cout << "Lines read: " << lines << std::endl;
    } else {
        std::cout << "File cannot be opened" << std::endl;
    }

    input_file.close();
    return data;
}

#endif
