#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

#include <iostream>

int main(int argc, char **argv) {

	std::cout << "test main called\n";

	return Catch::Session().run( argc, argv );
}
