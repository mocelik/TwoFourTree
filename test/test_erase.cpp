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

TEST_CASE( "Erase with single underflow", "[erase]" ) {
	tft::TwoFourTree<int> tree;
	tree.insert(20);
	tree.erase(20);
	CHECK(!tree.contains(20));
}

