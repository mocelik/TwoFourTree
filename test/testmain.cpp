#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

#include <iostream>
#include <chrono>

int main(int argc, char **argv) {

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	int rc = Catch::Session().run( argc, argv );
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	std::cout << "Tests finished in " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms" << std::endl;

	return rc;
}
