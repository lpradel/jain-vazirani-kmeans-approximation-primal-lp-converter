#include "LPConverter.h"

#include <iostream>
#include <sstream>

int main(int argc, char* argv[])
{
    if (argc != 5)
    {
        std::cout << "Please enter exactly 4 arguments!" << std::endl;
        std::cout << "Usage: inputFile delimiter k outputFile" << std::endl;
    }

	std::string inputFile(argv[1]);
    std::string delimiter_string(argv[2]);
    std::string k_string(argv[3]);
	std::string outputFile(argv[4]);

    int k = 0;
    std::istringstream ss(k_string);
    ss >> k;
    char delimiter = delimiter_string[0];

    std::cout << "InputFile: " << inputFile.c_str() << std::endl;
    std::cout << "delimiter: " << delimiter << std::endl;
    std::cout << "k: " << k << std::endl;
    std::cout << "outputFile: " << outputFile.c_str() << std::endl;

	ls2::LPConverter::convertToLP(inputFile, delimiter, k, outputFile);

    return 0;
}
