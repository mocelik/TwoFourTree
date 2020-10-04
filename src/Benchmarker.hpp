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

namespace tft {

class Benchmarker {
public:
	Benchmarker() = default;
	virtual ~Benchmarker() = default;

	void run();
};

} /* namespace tft */

#endif /* SRC_BENCHMARKER_HPP_ */
