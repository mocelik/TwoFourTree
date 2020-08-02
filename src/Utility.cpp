/*
 *  Filename:   Utility.cpp
 *
 *  Created on:  Aug. 1, 2020
 *      Author:  orhan
 *
 *  Description:
 *
 */

#include <Utility.hpp>

#include <algorithm>
#include <chrono>
#include <random>
#include <unordered_set>

namespace tft {

void trimSpaces(std::string& str) {
	if (str.empty())
		return;

	std::size_t firstScan = str.find_first_not_of(' ');
	std::size_t first = firstScan == std::string::npos ? str.length() : firstScan;
	std::size_t last = str.find_last_not_of(' ');
	str = str.substr(first, last - first + 1);
}

bool startsWith(const std::string & string, const std::string& search) {
	return string.rfind(search, 0) == 0;
}


std::unordered_set<int> generateUnique(std::unordered_set<int>::size_type size, int min, int max) {
	static std::mt19937_64 gen((std::random_device()()));
	std::uniform_int_distribution<> distribution(min, max);

	std::unordered_set<int> rc;
	while (rc.size() < size) {
		rc.insert(distribution(gen));
	}
	return rc;
}

} /** namespace tft */


