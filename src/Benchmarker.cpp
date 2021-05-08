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
#include <Utility.hpp>

#include <chrono>
#include <iostream>
#include <iomanip>
#include <vector>
#include <random>

namespace {
/**
 * Calls the callable object and returns the duration it took in microseconds
 */
template <typename Callable>
std::chrono::microseconds measure(Callable c ) {
	auto begin = std::chrono::steady_clock::now();
	c();
	auto end = std::chrono::steady_clock::now();

	return std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
}

}

namespace tft {

void Benchmarker::run() {
	clearTerminal();
	std::cout << "Benchmark code - in progress\n";

	while (true) {
		printSettings();

		std::cout << "Please choose one of the following options:\n"
				"d - change the random distribution type\n"
				"s - change the size of the benchmark (number of elements to insert/erase)\n"
				"r - change the range that elements can be selected from\n"
				"i - benchmark insertion\n"
				"e - benchmark erasing\n"
				"c - clear the tree and set\n"
				"q - quit\n";

		std::string input;
		std::getline(std::cin, input);
		if (!std::cin) {
			std::cerr << "IO Error\n";
			return;
		}

		clearTerminal();

		if (input == "d") {
			setDistribution();
		} else if (input == "s") {
			setSize();
		} else if (input == "r") {
			setRange();
		} else if (input == "i") {
			insert();
		} else if (input == "e") {
			erase();
		} else if (input == "c") {
			clear();
		} else if (input == "q") {
			return;
		} else {
			printTimes("No op", measure([](){}), measure([](){}));
		}

	}
}

std::vector<int> Benchmarker::generateRandom() const{
	return tft::generateRandom(bm_size_, 0 /* min value */, range_, distrib_);
}

void Benchmarker::printTimes(const std::string& operation, const std::chrono::microseconds& std_time, const std::chrono::microseconds& tft_time) const {
	std::cout << operation << " run times:\n"
			"Set         -  [" << std_time.count() << " us]\n"
			"TwoFourTree -  [" << tft_time.count() << " us]\n\n";
}

void Benchmarker::printSettings() const {
	std::cout << "Current settings are:\n";
	std::cout << "Distribution: " << distrib_ <<std::endl;
	std::cout << "Number of elements: " << bm_size_ << std::endl;
	std::cout << "Range (max value): " << range_ << std::endl;
	std::cout << "Current tree/set size: " << tree_.size() << std::endl;

	std::cout << std::endl << std::endl;
}

void Benchmarker::setDistribution(){

	std::cout << "Distributions:\n"
			"uni\t Uniform distribution\n"
			"b20\t Binomial Distribution with p = 0.20\n"
			"b50\t Binomial Distribution with p = 0.50\n"
			"b80\t Binomial Distribution with p = 0.80\n";

	std::string input;
	std::getline(std::cin, input);
	if (!std::cin)
		return;

	if (input ==  "uni")
		distrib_ = EDistribution::UNIFORM;
	else if (input == "b20")
		distrib_ = EDistribution::BINOMIAL_20;
	else if (input == "b50")
		distrib_ = EDistribution::BINOMIAL_50;
	else if (input == "b80")
		distrib_ = EDistribution::BINOMIAL_80;
}

void Benchmarker::setSize(){
	std::string input;
	std::cout << "How many elements should be used in the benchmark? (A higher number will take longer to run): ";
	while (true) {
		std::getline(std::cin, input);
		try {
			bm_size_ = std::stol(input);
			return;
		} catch (std::exception& e) {
			std::cout << e.what() << "\nPlease try again.\n";
		}
	}
}

void Benchmarker::setRange(){
	std::string input;
	std::cout << "What should the maximum value be? Recommend to set to 5x the benchmark size (" << 5*bm_size_ << "): ";
	while (true) {
		std::getline(std::cin, input);
		try {
			range_ = std::stoi(input);
			return;
		} catch (std::exception& e) {
			std::cout << e.what() << "\nPlease try again.\n";
		}
	}
}

void Benchmarker::insert(){
	auto test_values = generateRandom();

	auto set_size = set_.size();
	auto std_time = measure([this, &test_values]() {
		for (const auto& it : test_values)
			this->set_.insert(int(it));
	});
	if (set_size == set_.size())
		std::cerr << "Set size did not change after insert\n";

	auto tft_time = measure([this, &test_values]() {
		for (const auto &it : test_values)
			tree_.insert(int(it));
	});

	printTimes("Inserting " + std::to_string(bm_size_) + " elements", std_time, tft_time);
}

// Generate the list of numbers to be erased by iteratin  over the set and choosing numbers at random
// Then go through the tree and set while removing the chosen numbers
void Benchmarker::erase(){
	auto eraseSize = bm_size_ > set_.size() ? set_.size() : bm_size_;

	auto gen = generator();
	std::bernoulli_distribution bernoulli(static_cast<double>(eraseSize) / set_.size());
	std::vector<int> toErase;
	for (const auto& it : set_)
		if (bernoulli(gen))
			toErase.emplace(toErase.end(), it);

	// toErase now has an expected size of eraseSize
	// there will likely be a small deviation from eraseSize but that is acceptable

	// Since the set_ might already be cached because of recent access, we will first time
	// the tree.

	auto tft_time = measure([this, &toErase]() {
		for (const auto &it : toErase)
			tree_.erase(it);
	});

	auto std_time = measure([this, &toErase]() {
		for (const auto &it : toErase)
			set_.erase(it);
	});

	printTimes("Erasing " + std::to_string(toErase.size()) + " elements", std_time, tft_time);
}

void Benchmarker::clear(){
	auto std_time = measure([this]() {
		set_.clear();
	});

	auto tft_time = measure([this]() {
		tree_.clear();
	});

	printTimes("Clearing", std_time, tft_time);
}


} /* namespace tft */
