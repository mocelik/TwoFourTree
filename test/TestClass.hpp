/*
 *  Filename:   TestClass.hpp
 *
 *  Created on:  Jun. 20, 2020
 *      Author:  orhan
 *
 *  Description:
 *
 */

#ifndef TEST_TESTCLASS_HPP_
#define TEST_TESTCLASS_HPP_

class IntWrapper {
public:
	IntWrapper(int j) : i(j){}
	IntWrapper() : i(-1) {}
	~IntWrapper() {
		if (i == -1) {
			std::cout << "unused ";
		}
		std::cout << "dtor: " << this << std::endl;
	}
	friend std::ostream& operator<< (std::ostream& os, const IntWrapper& f) {
		os << f.i;
		return os;
	}
	friend IntWrapper& operator+ (IntWrapper& lhs, const int& rhs) {
		lhs.i += rhs;
		return lhs;
	}
	friend bool operator==(const int& lhs, const IntWrapper& rhs) {
		return lhs == rhs.i;
	}
	friend bool operator==(const IntWrapper& lhs, const int& rhs) {
		return lhs.i == rhs;
	}
	friend bool operator==(const IntWrapper& lhs, const IntWrapper& rhs) {
		return lhs.i == rhs.i;
	}
	friend bool operator<(const IntWrapper& lhs, const IntWrapper& rhs) {
		return lhs.i < rhs.i;
	}
	friend bool operator>(const IntWrapper& lhs, const IntWrapper& rhs) {
		return lhs.i > rhs.i;
	}
	int i;
};

#endif /* TEST_TESTCLASS_HPP_ */
