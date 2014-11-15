/*
 * LPConverter.cpp
 *
 *  Created on: Nov 13, 2014
 *      Author: lukas
 */

#include "LPConverter.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace ls2
{

LPConverter::LPConverter()
{
}

LPConverter::~LPConverter()
{
}

void LPConverter::convertToLP(const std::string &inputFile, const char delimiter, const std::string &outputFile)
{
	DoubleMat inputPointsMatrix;
	file2DoubleMat(inputFile, delimiter).swap(inputPointsMatrix);
    DoubleMat costs;
    calculateLPCosts(inputPointsMatrix).swap(costs);

	std::ofstream os;
	os.open(outputFile.c_str());

	if (os.fail())
	{
	    throw std::invalid_argument("File " + outputFile + " cannot be opened or created!");
	}
	if (!os.is_open())
	{
	    throw std::invalid_argument("File " + outputFile + " cannot be opened or created!");
	}

	std::cout << "Writing results to " << outputFile.c_str() << "..." << std::endl;
    writeLPToFile(os, costs);

	os.close();
}

DoubleMat LPConverter::calculateLPCosts(const DoubleMat& inputPoints)
{
	int n = inputPoints[0].size();

	DoubleMat costs(n, std::vector<double>(n, 0));

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			costs[i][j] = squaredEuclideanDistance(inputPoints[i], inputPoints[j]);
		}
	}

	return costs;
}

void LPConverter::writeLPToFile(std::ofstream& os, const DoubleMat& costs)
{
    int n = costs[0].size();

    // header
    os << "Minimize" << std::endl;
    os << "kmedian_2approximation:" << std::endl;

    // target function: sum c_ij*x_ij
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            os << costs[i][j] << "xi" << i << "j" << j;

            if ( (i == n-1) && (j == n-1) )
            {
                break;
            }

            os << " + ";
        }
    }
    os << std::endl;

    // restrictions header
    os << "Subject to" << std::endl;

    // restrictions: each city connected to at least one facility: sum x_ij >= 1

    // restrictions:

    // non-negativity relaxed restrictions
}

DoubleMat LPConverter::file2DoubleMat(const std::string &file, const char delimiter)
{
	std::ifstream is(file.c_str());

	if (is.fail())
	{
		throw std::invalid_argument("File " + file + " does not exist or cannot be opened!");
	}

	if (!is.is_open())
	{
		throw std::invalid_argument("File " + file + " does not exist or cannot be opened!");
	}

	DoubleMat doubleMat(0, std::vector<double>(0, 0));
	doubleMat.clear();

	std::string line;
	while (std::getline(is, line))
	{
		DoubleVec row(0);

		std::stringstream linestream(line);
		std::string value;
		while (std::getline(linestream, value, delimiter))
		{
			row.push_back(string2double(value));
		}
		doubleMat.push_back(row);
	}

	is.close();

	return doubleMat;
}

double LPConverter::string2double(const std::string &s)
{
	std::istringstream i(s);
	double x;

	if (!(i >> x))
		return 0;
	return x;
}

double LPConverter::squaredEuclideanDistance(const DoubleVec& v1, const DoubleVec& v2)
{
    if (v1.size() != v2.size())
    {
        throw std::invalid_argument("Euclidean distance can only be calculated for vectors of equal dimension!");
    }

    int dim = v1.size();
    double distance = 0.0;

    for (int i = 0; i < dim; i++)
    {
        distance += ( (v1[i] - v2[i]) * (v1[i] - v2[i]));
    }

    return distance;
}

} /* namespace ls2 */
