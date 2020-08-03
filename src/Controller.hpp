/*
 *  Filename:   Controller.h
 *
 *  Created on:  Jul. 25, 2020
 *      Author:  orhan
 *
 *  Description:
 *
 */

#ifndef SRC_CONTROLLER_HPP_
#define SRC_CONTROLLER_HPP_

namespace tft {

class Controller {
public:
	Controller() = default;
	virtual ~Controller() = default;

	int start(int argc, char **argv);
};

} /* namespace tft */

#endif /* SRC_CONTROLLER_HPP_ */
