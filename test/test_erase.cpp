/*
 *  Filename:   test_erase.cpp
 *
 *  Created on: May 16, 2020
 *      Author: orhan
 *
 *  Description:
 * 		Testing the erasing functionality of the TwoFourTree
 */

#include "../src/tftree.hpp"
#include "catch.hpp"

#include <cstdlib>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <chrono>

TEST_CASE( "Simple erase", "[erase]" ) {

	tft::TwoFourTree<int> tree;
	tree.insert(20);
	tree.insert(30);
	tree.insert(40);

	SECTION("Remove first") {
		tree.erase(20);
		CHECK(!tree.contains(20));
		tree.insert(20);
	}
	SECTION("Remove second") {
		tree.erase(30);
		CHECK(!tree.contains(30));
		tree.insert(30);
	}
	SECTION("Remove third") {
		tree.erase(40);
		CHECK(!tree.contains(40));
		tree.insert(40);
	}
	CHECK(tree.contains(20));
	CHECK(tree.contains(30));
	CHECK(tree.contains(40));

	tree.erase(40);
	tree.erase(30);
	CHECK(!tree.contains(40));
	CHECK(!tree.contains(30));
}

TEST_CASE( "Erase single empty node", "[erase]" ) {
	tft::TwoFourTree<int> tree;
	tree.insert(20);
	tree.erase(20);
	CHECK(!tree.contains(20));
}


/**
 *  Remove 76 from this tree:
 *           [ 52 ]
 *          /      \
 *  [ 20, 33 ]     [ 76 ]
 *
 *  Should cause a clockwise rotation so 33 goes up and 52 goes down to the right
 */
TEST_CASE( "Erase with clockwise rotation", "[erase][underflow]" ) {
	tft::TwoFourTree<int> tree;
	tree.insert(20);
	tree.insert(52);
	tree.insert(76);
	tree.insert(33);

	tree.erase(76);
	CHECK(!tree.contains(76));
	CHECK(tree.contains(20));
	CHECK(tree.contains(33));
	CHECK(tree.contains(52));
}

/**
 *  Remove 20 from this tree:
 *       [ 52 ]
 *      /      \
 *  [ 20 ]     [ 76, 82 ]
 *
 *  Should cause a counterclockwise rotation so 52 goes down and to the left, 76 goes up
 */
TEST_CASE( "Erase with counter clockwise rotation", "[erase][underflow]" ) {
	tft::TwoFourTree<int> tree;
	tree.insert(20);
	tree.insert(52);
	tree.insert(76);
	tree.insert(82);

	tree.erase(20);
	CHECK(!tree.contains(20));
	CHECK(tree.contains(52));
	CHECK(tree.contains(76));
	CHECK(tree.contains(82));
}

/**
 *  Remove 55 from this tree:
 *       [ 52, 60 ]
 *      /     |    \
 *  [ 20 ]  [ 55 ] [ 76 ]
 *
 *  20, 52 and 55 should merge into one node then 55 will be removed
 */
TEST_CASE( "Erase with parent fusion", "[erase][underflow][fusion]" ) {
	tft::TwoFourTree<int> tree;
	tree.insert(20);
	tree.insert(52);
	tree.insert(55);
	tree.insert(60);
	tree.insert(76);

	// insert and erase 80 to split the node and create the correct tree
	tree.insert(80);
	tree.erase(80);

	SECTION("Overflow left") {
		tree.erase(20);
		CHECK(!tree.contains(20));
		tree.insert(20); // so we can check en-masse at the end of the section
	}
	SECTION("Overflow middle") {
		tree.erase(55);
		CHECK(!tree.contains(55));
		tree.insert(55); // so we can check en-masse at the end of the section
	}
	SECTION("Overflow right") {
		tree.erase(76);
		CHECK(!tree.contains(76));
		tree.insert(76); // so we can check en-masse at the end of the section
	}

	CHECK(tree.contains(20));
	CHECK(tree.contains(52));
	CHECK(tree.contains(55));
	CHECK(tree.contains(60));
	CHECK(tree.contains(76));
}

