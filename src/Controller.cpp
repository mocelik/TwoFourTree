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

#include <iostream>

namespace tft {

int Controller::start(int argc, char **argv) {
	std::cout << "Controller\n";
	std::cout << "params: " << argc << ' ';
	for (int i =0; i < argc; i++)
		std::cout << argv[i] << ' ';
	std::cout << std::endl;

	InteractiveTester ui;
	ui.run();

	return 0;
}

} /* namespace tft */
