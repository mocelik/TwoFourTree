/*
 *  Filename:   Controller.cpp
 *
 *  Created on:  Jul. 25, 2020
 *      Author:  orhan
 *
 *  Description:
 *
 */

#include "Controller.hpp"

#include "tftree.hpp"
#include "InteractiveTester.hpp"
#include "Benchmarker.hpp"
#include "Utility.hpp"

#include <iostream>

namespace tft {

int Controller::start(int argc, char **argv) {
	Benchmarker bm;
	InteractiveTester ui;

	while (true) {
		clearTerminal();

		std::cout << "b or benchmark to enter benchmark mode\n"
					 "i or interactive to enter interactive mode\n"
					 "h or help to read a help message\n"
					 "q or quit to exit\n";

		std::string input;
		std::getline(std::cin, input);
		if (!std::cin) {
			std::cout << "IO error. Exiting\n";
			return EXIT_FAILURE;
		}

		trimSpaces(input);

		if (input == "b" || input == "benchmark") {
			bm.run();
		} else if (input == "i" || input == "interactive") {
			ui.run();
		} else if (input == "h" || input == "help") {
			std::cout << "TODO: help message\n";
		} else if (input == "q" || input == "quit") {
			std::cout << "Bye!\n";
			return EXIT_SUCCESS;
		}

		if (!std::cin)
			return EXIT_FAILURE;
	}
}

} /* namespace tft */
