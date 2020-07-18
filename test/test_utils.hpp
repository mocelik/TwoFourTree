/*
 *  Filename:   test_utils.hpp
 *
 *  Created on:  Jul. 15, 2020
 *      Author:  orhan
 *
 *  Description:
 *
 */

#ifndef TEST_TEST_UTILS_HPP_
#define TEST_TEST_UTILS_HPP_

#include <unordered_set>
#include <vector>

/**
 * Returns an unordered_set containing SIZE unique numbers with a uniform distribution
 * The returned set will not contain any duplicates of the EXCLUDING set
 */
std::unordered_set<int> generateUnique(int size, const std::unordered_set<int>& excluding = {});

/** 
 * Returns a vector with random values from 0 to RANGE 
 */
std::vector<int> randomizeSerial(std::vector<int>::size_type range);

#endif /* TEST_TEST_UTILS_HPP_ */
