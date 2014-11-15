/*
 * LPConverter.h
 *
 *  Created on: Nov 13, 2014
 *      Author: lukas
 */

#ifndef LPCONVERTER_H_
#define LPCONVERTER_H_

#include <fstream>
#include <string>
#include <vector>

/**
 * \namespace ls2
 * \brief Technische Universitaet Dortmund, Dpt. of Computer Science, Chair 2.
 */
namespace ls2
{

typedef std::vector<std::vector<double> > DoubleMat;
typedef std::vector<double> DoubleVec;

class LPConverter
{
public:
	virtual ~LPConverter();

	static void convertToLP(const std::string &inputFile, const char delimiter, const std::string &outputFile);

private:
	LPConverter();
	static DoubleMat calculateLPCosts(const DoubleMat& inputPoints);
	static DoubleMat file2DoubleMat(const std::string &file, const char delimiter);
    static void writeLPToFile(std::ofstream& os, const DoubleMat& costs);
	static double string2double(const std::string &s);
    static double squaredEuclideanDistance(const DoubleVec& v1, const DoubleVec& v2);
};

} /* namespace ls2 */

#endif /* LPCONVERTER_H_ */
