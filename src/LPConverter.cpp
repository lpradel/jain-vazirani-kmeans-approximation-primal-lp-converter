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

void LPConverter::convertToLP(const std::string &inputFile, const char delimiter, int k, const std::string &outputFile)
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
    writeLPToFile(os, costs, k);

    os.close();
}

DoubleMat LPConverter::calculateLPCosts(const DoubleMat& inputPoints)
{
    int n = inputPoints.size();
    
    std::cout <<  "N: " << n << std::endl;

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

void LPConverter::writeLPToFile(std::ofstream& os, const DoubleMat& costs, int k)
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
            os << std::fixed << costs[i][j] << "xi" << i << "j" << j;

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

    // restrictions: each city connected to at least one facility: foreach j in C sum x_ij >= 1
    for (int j = 0; j < n; j++)
    {
        //os << "City_" << j << "_connected: ";
        for (int i = 0; i < n; i++)
        {
            os << "xi" << i << "" << "j" << j;

            if (i != n-1)
            {
                os << " + ";
            }
            else
            {
                os << " >= 1";
            }
        }
        os << std::endl;
    }

    // restrictions: connected facilities must be open: foreach i in F, j in C y_i - x_ij >= 0
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            //os << "Facility_" << i << "_open_rsp_city_" << j << ": ";
            os << "yi" <<  i << " - " << "xi" << i << "j" << j << " >= 0" << std::endl;
        }
    }

    // restrictions: exactly k facilities are opened: sum -y_i >= -k
    //os << "Exactly_k_facilities_opened: ";
    for (int i = 0; i < n; i++)
    {
        os << "-yi" << i << " ";
    }
    os << " >= " << "-" << k << std::endl;

    // relaxed ILP-restrictions i.e. non-negativity: foreach i in F, j in C x_ij >= 0
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            os << "xi" << i << "j" << j << " >= 0" << std::endl;
        }
    }
    // foreach i in F y_i >= 0
    for (int i = 0; i < n; i++)
    {
        os << "yi" << i << " >= 0" << std::endl;
    }

    // footer
    os << "End" << std::endl;
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
