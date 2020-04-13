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

	// first few values will go into root
	REQUIRE(tree.insert(30).second);
	REQUIRE(tree.insert(40).second);
	REQUIRE(tree.insert(20).second);

//	SECTION("Simple overflow") {
//		REQUIRE(tree.insert(10).second);
//		REQUIRE(tree.insert(15).second);
//		REQUIRE(tree.insert(25).second); // this one will cause overflow
//		std::cout << "End of Simple Overflow:\n";
//		tree.print();
//	}
//
//	SECTION("Cascading Overflow") {
//		REQUIRE(tree.insert(10).second);
//		REQUIRE(tree.insert(15).second);
//		REQUIRE(tree.insert(25).second); // this one will cause simple overflow
//
//		REQUIRE(tree.insert(18).second);
//		REQUIRE(tree.insert(19).second);
//		REQUIRE(tree.insert(16).second);
//
//		std::cout << "Printing before cascaded overflow:\n";
//		tree.print();
//
//		REQUIRE(tree.insert(1).second); // fails <- cascaded overflow
//		std::cout << "Printing after cascaded overflow:\n";
//		tree.print();
//	}

}

