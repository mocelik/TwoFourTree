/*
 *  Filename:   Benchmarker.cpp
 *
 *  Created on:  Jul. 25, 2020
 *      Author:  orhan
 *
 *  Description:
 *
 */

#include <Benchmarker.hpp>

#include <tftree.hpp>
#include <DataGenerator.hpp>

#include <chrono>
#include <iostream>
#include <iomanip>
#include <limits>
#include <set>

namespace tft {

void Benchmarker::run() {
	std::cout << "Benchmark code - in progress\n";

	TwoFourTree<int> tree;
	std::set<int> set;

	std::cout << "Size?\n";
	int size;
	std::cin >> size;

	auto test_values = generateRandom(size, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), EDistribution::BINOMIAL_50);

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	for (const auto& it : test_values)
		set.insert(int(it));
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << std::setw(15) << "set took: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " us\n";

	begin = std::chrono::steady_clock::now();
	for (const auto& it : test_values)
		tree.insert(int(it));
	end = std::chrono::steady_clock::now();
	std::cout << std::setw(15) << "tree took: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " us\n";

}

} /* namespace tft */
