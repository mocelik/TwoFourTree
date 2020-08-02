/*
 *  Filename:   Utility.hpp
 *
 *  Created on:  Aug. 1, 2020
 *      Author:  orhan
 *
 *  Description:
 *
 */

#ifndef SRC_UTILITY_HPP_
#define SRC_UTILITY_HPP_

#include <string>
#include <unordered_set>

namespace tft {

/**
 * Removes leading and trailing spaces
 */
void trimSpaces(std::string &str);

/**
 * Returns true if the provided string starts with the provided search
 */
bool startsWith(const std::string & string, const std::string& search);

/**
 * Returns a set of unique numbers
 */
std::unordered_set<int> generateUnique(std::unordered_set<int>::size_type size, int min = 0, int max = 100);

} /** namespace tft */



#endif /* SRC_UTILITY_HPP_ */
