/*
 *  Filename:   DataGenerator.h
 *
 *  Created on:  Jul. 25, 2020
 *      Author:  orhan
 *
 *  Description:
 *
 */

#ifndef SRC_DATAGENERATOR_HPP_
#define SRC_DATAGENERATOR_HPP_

#include <unordered_set>
#include <iostream>
#include <random>

namespace tft {

// Types of distributions directly corresponding to the distribution types in <random>
enum class EDistribution {
	UNIFORM,
	BINOMIAL_20,
	BINOMIAL_50,
	BINOMIAL_80
};

std::ostream& operator<<(std::ostream&, const EDistribution&);

// Returns the mersenne_twister_engine object used to generate random values
std::mt19937& generator();

/**
 * Returns an unordered_set of randomly generated numbers
 *
 * \param size	The number of numbers to be generated and returned
 * \param min	The smallest value that a random number should be
 * \param max	The maximum value that a random number should be
 * \param dist	The statistical distribution to choose when generating random numbers
 */
std::unordered_set<int> generateRandom(std::unordered_set<int>::size_type size, int min, int max, EDistribution dist);

} /* namespace tft */

#endif /* SRC_DATAGENERATOR_HPP_ */
