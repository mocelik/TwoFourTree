/*
 *  Filename:   Benchmarker.h
 *
 *  Created on:  Jul. 25, 2020
 *      Author:  orhan
 *
 *  Description:
 *
 */

#ifndef SRC_BENCHMARKER_HPP_
#define SRC_BENCHMARKER_HPP_

#include "tftree.hpp"
#include "DataGenerator.hpp"

#include <unordered_set>
#include <set>
#include <chrono>
#include <string>

namespace tft {

class Benchmarker {
public:
	Benchmarker() = default;
	virtual ~Benchmarker() = default;

	void run();

private:
	std::vector<int> generateRandom() const;
	void printTimes(const std::string &operation,
			const std::chrono::microseconds &std_time,
			const std::chrono::microseconds &tft_time) const;
	void printSettings() const;

	void setDistribution();
	void setSize();
	void setRange();
	void insert();
	void erase();
	void find();
	void clear();
	void sampleBenchmark();

	TwoFourTree<int> tree_;
	std::set<int> set_;
	EDistribution distrib_ { EDistribution::UNIFORM };
	TwoFourTree<int>::size_type bm_size_ { 10000 };
	int range_ { 50000 };
};

} /* namespace tft */

#endif /* SRC_BENCHMARKER_HPP_ */
