/*
 *  Filename:   InteractiveTester.cpp
 *
 *  Created on:  Jul. 25, 2020
 *      Author:  orhan
 *
 *  Description:
 *
 */

#include <InteractiveTester.hpp>
#include <Utility.hpp>

#include <tftree.hpp>

#include <string>
#include <sstream>

namespace tft {

void InteractiveTester::add(const std::string& line){
	std::stringstream ss(line);

	std::string str;
	ss >> str;

	if (str != "add")
		return;

	ss >> str;
	if (str == "random") {
		std::cout << "unhandled add random";
		return;
	}

	try {
		int num = std::stoi(str);
		ss >> str;

		if (str == "to") {
			ss >> str;
			int start = num;
			int end = std::stoi(str);

			for (int i = start; i < end; i++)
				tree_.insert(int(i));
			return;
		} else {
			tree_.insert(int(num));
			tree_.insert(std::stoi(str));

			while (ss >> num)
				tree_.insert(int(num));
		}
	} catch (std::exception& e) {
		std::cout << "Expected number\n";
	}
}

void InteractiveTester::erase(const std::string& line){
	std::cout << "Entering function: " << __FUNCTION__ << " with line " << line <<std::endl;
}
void InteractiveTester::add_multi(const std::string& line){
	std::cout << "Entering function: " << __FUNCTION__ << " with line " << line <<std::endl;
}
void InteractiveTester::clear(const std::string& line) {
	std::cout << "Entering function: " << __FUNCTION__ << " with line " << line << std::endl;
}

void InteractiveTester::resetTerminal() {
	std::cout << "\033[2J"; // clear terminal
	std::cout << "\033[1;1H"; // position cursor at 1:1
}

void InteractiveTester::run() {
	resetTerminal();

	while( true ) {

		int instruction_width = 23;
		std::cout << tree_ << std::endl;
		std::cout << "On the left are commands with their effects on the tree described to the right:" << std::endl
				<< std::left
				<< std::setw(instruction_width) << "  add 1 2 3 4" 	<< "adds 1,2,3, and 4" << std::endl
				<< std::setw(instruction_width) << "  add 0 to 5" 	<< "adds [0, 5)" << std::endl
				<< std::setw(instruction_width) << "  add random 5" 	<< "adds 5 random numbers " << std::endl
				<< std::setw(instruction_width) << "  remove 1 2 3" 	<< "removes 1,2 and 3" << std::endl
				<< std::setw(instruction_width) << "  remove 0 to 5" << "removes [0, 5)" << std::endl
				<< std::setw(instruction_width) << "  clear" 		<< "removes all values from the tree, resetting it" << std::endl
				<< std::setw(instruction_width) << "  q" 			<< "quit" << std::endl;

		std::cout << "Please enter a properly formatted instruction now: ";
		std::string line;
		std::getline(std::cin, line);

		trimSpaces(line);

		if (!std::cin) {
			if (std::cin.bad()) {
				std::cerr << "I/O error\n";
				return;
			} else if (std::cin.eof()) {
				std::cerr << "Unexpected end of file\n";
				return;
			} else if (std::cin.fail()) {
				std::cerr << "Unexpected error\n";
				std::cin.clear();
				return;

			} else {
				std::cerr << "Unknown error when reading\n";
				return;
			}
		}

		if (startsWith(line, "add")) {
			add(line);
		} else if (startsWith(line, "erase")) {
			erase(line);
		} else if (startsWith(line, "clear")) {
			clear(line);
		} else if (line == "q") {
			return;
		}
		resetTerminal();
	}
}

} /* namespace tft */
