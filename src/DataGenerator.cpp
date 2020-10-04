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

std::mt19937 generator() {
	static std::mt19937 gen((std::random_device()()));
	return gen;
}

template <typename T>
std::unordered_set<int> generate(T& distrib, size_type size) {
	auto gen = generator();
	std::unordered_set<int> rc;

	while (rc.size() < size) {
		rc.insert(distrib(gen));
	}
	return rc;
}

std::unordered_set<int> randomUniform(size_type size, int min, int max) {
	std::uniform_int_distribution<> distribution(min, max);
	return generate<>(distribution, size);
}

// binomial only returns positive integers starting from 0
// so if the start value is non-zero then the set is corrected
std::unordered_set<int> randomBinomial(double p, size_type size, int min, int max) {
	if (min == 0) {
		std::binomial_distribution<int> distribution(max, p);
		return generate<>(distribution, size);

	} else {
		int range = max - min;

		std::binomial_distribution<int> distribution(range, 0.5);
		auto skewedSet = generate<>(distribution, size);

		std::unordered_set<int> rc;
		for (const auto& skewedValue : skewedSet)
			rc.insert(skewedValue + min);

		return rc;
	}
}

}

namespace tft {


std::unordered_set<int> generateRandom(size_type size, int min, int max, EDistribution dist) {
	if (static_cast<size_type>(max - min) >= size) {
		std::unordered_set<int> rc;
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

} /* namespace tft */
