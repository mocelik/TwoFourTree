#include <Controller.hpp>
#include <iostream>

#include "tftree.hpp"

int main(int argc, char **argv) {
	tft::Controller controller;
	return controller.start(argc, argv);
}
