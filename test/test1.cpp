/*
 *  Filename:   test1.cpp
 *
 *  Created on: Mar. 22, 2020
 *      Author: orhan
 *
 *  Description:
 * 		Temporary file to be updated with some test cases
 */

#include "catch.hpp"
#include "../src/tftree.hpp"

TEST_CASE( "Inserting", "[insert]" ) {

	tft::TwoFourTree<int> tree;

	std::cout << "begin inserting.\n";
	REQUIRE(tree.insert(0).second);
	REQUIRE(tree.insert(1).second);
	REQUIRE(tree.insert(2).second);
	std::cout << "done inserting.\n";
	tree.print();

	SECTION("Simple overflow") {
		REQUIRE(tree.insert(3).second);
	}
}

