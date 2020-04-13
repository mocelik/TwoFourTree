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
#include <vector>

TEST_CASE( "Simple Insert", "[insert]" ) {

	tft::TwoFourTree<int> tree;

	// first few values will go into root
	REQUIRE(tree.insert(30).second);
	REQUIRE(tree.insert(40).second);
	REQUIRE(tree.insert(20).second);

	// Adding same values again should not succeed
	REQUIRE(!tree.insert(30).second);
	REQUIRE(!tree.insert(40).second);
	REQUIRE(!tree.insert(20).second);
}

/**
 *  I want this tree:
 *              [   30,          40     ]
 *             /            |             \
 *     [ 13, 15, 17]  [ 33, 35, 37]  [ 43, 45, 37]
 *
 *  And I will overflow each of the 3 children by adding 14, 34 and 44 respectively
 */
TEST_CASE( "Insert with Single Overflow", "[insert]" ) {

	tft::TwoFourTree<int> tree;

	// first few values will go into root
	std::vector<int> values {13, 30, 40, 47, 35, 33, 15, 17, 37, 43, 45 };
	bool rc;
	for ( auto it : values ) {
		rc = tree.insert(std::move(it)).second;
		REQUIRE(rc);
	}

	SECTION("First child overflows") {
		rc = tree.insert(14).second;
		CHECK(rc);
	}

	SECTION("Second child overflows") {
		rc = tree.insert(34).second;
		CHECK(rc);
	}

	SECTION("Third child overflows") {
		rc = tree.insert(44).second;
		CHECK(rc);
	}

	if (!rc) {
		tree.print();
	}
}

