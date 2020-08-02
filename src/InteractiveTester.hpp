/*
 *  Filename:   InteractiveTester.h
 *
 *  Created on:  Jul. 25, 2020
 *      Author:  orhan
 *
 *  Description:
 *
 */

#ifndef SRC_INTERACTIVETESTER_HPP_
#define SRC_INTERACTIVETESTER_HPP_

#include <map>
#include <string>
#include <functional>

#include "tftree.hpp"

namespace tft {

class InteractiveTester {
public:
	InteractiveTester() = default;
	virtual ~InteractiveTester() = default;

	void run();

private:
	void resetTerminal();

	void add(const std::string& line);
	void erase(const std::string& line);
	void clear();

	TwoFourTree<long> tree_;
};

} /* namespace tft */

#endif /* SRC_INTERACTIVETESTER_HPP_ */
