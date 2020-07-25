/*
 *  Filename:   test1.cpp
 *
 *  Created on: Mar. 22, 2020
 *      Author: orhan
 *
 *  Description:
 * 		Temporary file to be updated with some test cases
 */

#include <tftree.hpp>
#include "catch.hpp"

#include <cstdlib>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <chrono>

TEST_CASE( "Insert single node", "[insert]" ) {

	tft::TwoFourTree<int> tree;

	// first few values will go into root
	REQUIRE(tree.insert(30).second);
	REQUIRE(tree.insert(40).second);
	REQUIRE(tree.insert(20).second);

	CHECK(tree.contains(20));
	CHECK(tree.contains(30));
	CHECK(tree.contains(40));

	// Adding same values again should not succeed
	CHECK(!tree.insert(30).second);
	CHECK(!tree.insert(40).second);
	CHECK(!tree.insert(20).second);

	REQUIRE(tree.validate());
}

/**
 *  I want this tree:
 *              [   30,          40     ]
 *             /            |             \
 *     [ 13, 15, 17]  [ 33, 35, 37]  [ 43, 45, 37]
 *
 *  And I will overflow each of the 3 children by adding 14, 34 and 44 respectively
 */
TEST_CASE( "Insert with Single Overflow", "[insert][overflow]" ) {

	tft::TwoFourTree<int> tree;

	// order is very important to get the specified tree
	std::vector<int> values {13, 30, 40, 47, 35, 33, 15, 17, 37, 43, 45 };
	bool rc;
	for ( auto it : values ) {
		rc = tree.insert(std::move(it)).second;
		REQUIRE(rc);
	}

	SECTION("First child overflows") {
		rc = tree.insert(14).second;
		CHECK(rc);
		CHECK(tree.contains(14));
	}

	SECTION("Second child overflows") {
		rc = tree.insert(34).second;
		CHECK(rc);
		CHECK(tree.contains(34));
	}

	SECTION("Third child overflows") {
		rc = tree.insert(44).second;
		CHECK(rc);
		CHECK(tree.contains(44));
	}

	if (!rc) {
		tree.print();
	}

	for ( auto it : values ) {
		REQUIRE(tree.contains(it));
	}

	REQUIRE(tree.validate());
}


/**
 *  Cascaded overflow test
 *  Add more values to the tree from above to make the parent full
 *
 *  This is the tree used:
 *             [    30,            40               50  ]
 *            /             |              |             \
 *     [ 13, 15, 17 ] [ 33, 35, 37 ] [ 43, 45, 37 ] [ 53, 55, 57 ]
 *
 *  This test overflows each of the 4 children by adding 14, 34, 44 and 54 respectively
 *  Expected behaviour is for 40 to be pushed to become the new root (height increase by one)
 */
TEST_CASE( "Insert with Cascaded Overflow", "[insert][overflow]" ) {

	tft::TwoFourTree<int> tree;

	// order is very important to get the specified tree
	std::vector<int> values {13, 30, 40, 47, 35, 33, 15, 17, 37, 50, 55, 43, 45, 53, 57 };
	bool rc;
	for ( auto it : values ) {
		rc = tree.insert(std::move(it)).second;
		REQUIRE(rc);
	}

	SECTION("First child overflows") {
		rc = tree.insert(14).second;
		CHECK(rc);
		CHECK(tree.contains(14));
		rc = tree.insert(14).second;
		CHECK(!rc);
	}

	SECTION("Second child overflows") {
		rc = tree.insert(34).second;
		CHECK(rc);
		CHECK(tree.contains(34));
		rc = tree.insert(34).second;
		CHECK(!rc);
	}

	SECTION("Third child overflows") {
		rc = tree.insert(44).second;
		CHECK(rc);
		CHECK(tree.contains(44));
		rc = tree.insert(44).second;
		CHECK(!rc);
	}

	SECTION("Fourth child overflows") {
		rc = tree.insert(54).second;
		CHECK(rc);
		CHECK(tree.contains(54));
		rc = tree.insert(54).second;
		CHECK(!rc);
	}

	for ( auto it : values ) {
		REQUIRE(tree.contains(it));
	}

	REQUIRE(tree.validate());
}

TEST_CASE( "Insert - Stress test with random adds", "[insert][overflow]" ) {
	const int numTests = 100000; // 100k
	std::unordered_set<int> rands( numTests ); // need set to remove duplicates
	for (int i = 0; i < numTests; i++)
		rands.insert(std::move(rand()));

	tft::TwoFourTree<int> tree;

	for (auto it : rands ) {
		REQUIRE(tree.insert(std::move(it)).second);
	}

	for (auto it : rands) {
		REQUIRE(tree.contains(it));
	}

	REQUIRE(tree.validate());
}
