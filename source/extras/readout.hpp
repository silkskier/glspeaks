#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <boost/spirit/include/qi.hpp>

struct photometry {
    std::vector<double> x; // Change to double
    std::vector<float> y;
    std::vector<float> dy;
};

photometry read_dat(const std::string& in_file) {
    photometry data;
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

        while (it < end) {
            std::vector<double> tempVec; // Change to double
            const char* line_end = std::find(it, end, '\n');

            bool success = boost::spirit::qi::phrase_parse(it, line_end, *boost::spirit::qi::double_ >> *(boost::spirit::qi::lit(',') >> boost::spirit::qi::float_), boost::spirit::qi::space, tempVec);

            if (success && it == line_end) {
                data.x.push_back(tempVec[0]);
                data.y.push_back(tempVec[1]);
                data.dy.push_back(tempVec[2]);
                it = line_end + 1; // Move to the next line
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
