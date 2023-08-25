#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <boost/spirit/include/qi.hpp>

struct photometry {
    std::vector<float> x;
    std::vector<float> y;
    std::vector<float> dy;
};

inline photometry read_dat(const std::string& in_file) {
    photometry photometry;
    std::ifstream input_file(in_file);

    if (input_file) {
        std::string line;
        while (std::getline(input_file, line)) {
            std::vector<float> tempVec;
            std::string::const_iterator it = line.begin();
            std::string::const_iterator end = line.end();

            bool success = boost::spirit::qi::phrase_parse(it, end, *boost::spirit::qi::float_ >> *(boost::spirit::qi::lit(',') >> boost::spirit::qi::float_), boost::spirit::qi::space, tempVec);

            if (success && it == end) {
                photometry.x.push_back(tempVec[0]);
                photometry.y.push_back(tempVec[1]);
                photometry.dy.push_back(tempVec[2]);
            } else {
                std::cout << "Parsing failed" << std::endl;
                break;
            }
        }
    } else {
        std::cout << "File cannot be opened" << std::endl;
    }

    input_file.close();
    return photometry;
}
