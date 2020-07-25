/*
 *  Filename:   test_utils.cpp
 *
 *  Created on:  Jul. 15, 2020
 *      Author:  orhan
 *
 *  Description:
 *
 */

#include "test_utils.hpp"

#include <algorithm>
#include <chrono>
#include <random>
#include <unordered_set>

std::unordered_set<int> generateUnique(std::unordered_set<int>::size_type size, const std::unordered_set<int>& excluding) {
	std::random_device rd;
	std::mt19937_64 gen(rd());
	std::uniform_int_distribution<> distribution;

	std::unordered_set<int> rc;
	while (rc.size() < size) {
		auto rand = distribution(gen);
		if (excluding.count(rand) == 0)
			rc.insert(rand);
	}

	return rc;
}


std::vector<int> randomizeSerial(std::vector<int>::size_type range){

	std::vector<int> rc(range);
	std::iota(rc.begin(), rc.end(), 0);
	std::shuffle(rc.begin(), rc.end(),
			std::default_random_engine(
					std::chrono::system_clock::now().time_since_epoch().count()));

	return rc;
}
