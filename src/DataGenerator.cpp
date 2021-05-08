/*
 *  Filename:   DataGenerator.cpp
 *
 *  Created on:  Jul. 25, 2020
 *      Author:  orhan
 *
 *  Description:
 *
 */

#include <DataGenerator.hpp>

#include <algorithm>
#include <random>

namespace {

using size_type = std::unordered_set<int>::size_type;

template <typename T>
std::vector<int> generate(T& distrib, size_type size) {
	std::vector<int> rc(size);
	std::generate(rc.begin(), rc.end(), [&distrib](){ return distrib(tft::generator()); });
	return rc;
}

std::vector<int> randomUniform(size_type size, int min, int max) {
	std::uniform_int_distribution distribution(min, max);
	return generate(distribution, size);
}

// binomial only returns positive integers starting from 0
// so if the start value is non-zero then the set is corrected
std::vector<int> randomBinomial(double p, size_type size, int min, int max) {
	if (min == 0) {
		std::binomial_distribution<int> distribution(max, p);
		return generate(distribution, size);

	} else {
		int range = max - min;

		std::binomial_distribution<int> distribution(range, p);
		auto rc = generate(distribution, size);

		std::transform(rc.begin(), rc.end(), rc.begin(), [min](int i){return i+min; } );

		return rc;
	}
}

}

namespace tft {

std::mt19937& generator() {
	static std::mt19937 gen((std::random_device()()));
	return gen;
}

std::vector<int> generateRandom(size_type size, int min, int max, EDistribution dist) {
	if (min > max) {
		auto tmp = min;
		min = max;
		max = tmp;
	}
	if (static_cast<size_type>(max - min) <= size) {
		std::vector<int> rc;
		int i = min;
		std::generate_n(std::inserter(rc, rc.begin()), size, [&i]() { return i++; });
		return rc;
	}

	switch (dist) {
	case EDistribution::UNIFORM:
		return randomUniform(size, min, max);
	case EDistribution::BINOMIAL_20:
		return randomBinomial(0.2, size, min, max);
	case EDistribution::BINOMIAL_50:
		return randomBinomial(0.5, size, min, max);
	case EDistribution::BINOMIAL_80:
		return randomBinomial(0.8, size, min, max);

	default:
		return randomUniform(size, min, max);
	}
}


std::ostream& operator<<(std::ostream& os, const EDistribution& dist){

	switch (dist) {
	case EDistribution::UNIFORM:
		os << "UNIFORM";
		return os;
	case EDistribution::BINOMIAL_20:
		os << "BINOMIAL_20";
		return os;
	case EDistribution::BINOMIAL_50:
		os << "BINOMIAL_50";
		return os;
	case EDistribution::BINOMIAL_80:
		os << "BINOMIAL_80";
		return os;
	default:
		os << "UNKNOWN_DISTRIBUTION";
		return os;
	}
}

} /* namespace tft */
