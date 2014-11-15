#include "LPConverter.h"

#include <iostream>

int main(int argc, char* argv[])
{

	std::string inputFile("D://mini.data");
	std::string outputFile("D://mini.lp");

	ls2::LPConverter::convertToLP(inputFile, ' ', 2, outputFile);

    return 0;
}
