#include "LPConverter.h"

#include <iostream>

int main(int argc, char* argv[])
{

	std::string inputFile("D://mini.data");
	std::string outputFile("D://mini.lp");

	ls2::LPConverter::convertToLP(inputFile, ' ', outputFile);

    return 0;
}
