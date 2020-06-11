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
TEST_CASE( "Erase with transfer left", "[erase][transferFromLeft]" ) {
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
TEST_CASE( "Erase transfer right", "[erase][transferFromRight]" ) {
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
 *  Remove from the leaves of this tree:
 *       [ 52,  60,  80 ]
 *      /     |     |    \
 *  [ 20 ] [ 55 ] [ 76 ]  [ 81 ]
 *
 *  The root should give away one of its values to the node with the erased value
 */
TEST_CASE( "Erase fusion", "[erase][fusion]" ) {
	tft::TwoFourTree<int> tree;
	tree.insert(20);
	tree.insert(52);
	tree.insert(55);
	tree.insert(60);
	tree.insert(76);
	tree.insert(80);
	tree.insert(81);

	// insert and erase 82 to split the node and create the correct tree
	tree.insert(82);
	tree.erase(82);

	SECTION("Fuse from child 0") {
		tree.erase(20);
		CHECK(!tree.contains(20));

		CHECK(tree.contains(52));
		CHECK(tree.contains(55));
		CHECK(tree.contains(60));
		CHECK(tree.contains(76));
		CHECK(tree.contains(80));
		CHECK(tree.contains(81));
	}
	SECTION("Fuse from child 1") {
		tree.erase(55);
		CHECK(!tree.contains(55));

		CHECK(tree.contains(20));
		CHECK(tree.contains(52));
		CHECK(tree.contains(60));
		CHECK(tree.contains(76));
		CHECK(tree.contains(80));
		CHECK(tree.contains(81));
	}
	SECTION("Fuse from child 2") {
		tree.erase(76);
		CHECK(!tree.contains(76));

		CHECK(tree.contains(20));
		CHECK(tree.contains(52));
		CHECK(tree.contains(55));
		CHECK(tree.contains(60));
		CHECK(tree.contains(80));
		CHECK(tree.contains(81));
	}
	SECTION("Fuse from child 3") {
		tree.erase(81);
		CHECK(!tree.contains(81));

		CHECK(tree.contains(20));
		CHECK(tree.contains(52));
		CHECK(tree.contains(55));
		CHECK(tree.contains(60));
		CHECK(tree.contains(76));
		CHECK(tree.contains(80));
	}
}

/**
 * Fusion from the parent or from a tree with only 3 nodes (parent and two children) is a special case
 * Removing any of the values in this tree should fuse the 3 nodes into one node:
 * 	   [1]
 *  [0]   [2]
 */
TEST_CASE( "Erase fusion root", "[erase][fusion]" ) {
	tft::TwoFourTree<int> tree;
	tree.insert(0);
	tree.insert(1);
	tree.insert(2);

	// add another one to split the nodes up, then remove
	tree.insert(3);
	tree.erase(3);

	SECTION("Erase left") {
		tree.erase(0);
		CHECK(!tree.contains(0));
		tree.insert(0);
	}

	SECTION("Erase right") {
		tree.erase(2);
		CHECK(!tree.contains(2));
		tree.insert(2);
	}

	SECTION("Erase root") {
		tree.erase(1);
		CHECK(!tree.contains(1));
		tree.insert(1);
	}

	CHECK(tree.contains(0));
	CHECK(tree.contains(1));
	CHECK(tree.contains(2));
}


/**
 *  Erase internal values in root
 *
 *  This is the tree used:
 *             [    30,            40               50  ]
 *            /             |              |             \
 *     [ 13, 15, 17 ] [ 33, 35, 37 ] [ 43, 45, 37 ] [ 53, 55, 57 ]
 *
 * Verify that erasing 30, 40 and 50 works
 */
TEST_CASE( "Erase internal", "[erase][internal][swap]" ) {
	tft::TwoFourTree<int> tree;

	// order is very important to get the specified tree
	std::vector<int> values {13, 30, 40, 47, 35, 33, 15, 17, 37, 50, 55, 43, 45, 53, 57 };
	bool rc;
	for ( auto it : values ) {
		rc = tree.insert(std::move(it)).second;
		REQUIRE(rc);
	}

	SECTION("Erase first") {
		tree.erase(30);
		CHECK(!tree.contains(30));
		values.erase(std::find(values.begin(), values.end(), 30));
	}

	SECTION("Erase second") {
		tree.erase(40);
		CHECK(!tree.contains(40));
		values.erase(std::find(values.begin(), values.end(), 40));
	}

	SECTION("Erase third") {
		tree.erase(50);
		CHECK(!tree.contains(50));
		values.erase(std::find(values.begin(), values.end(), 50));
	}

	for ( auto it : values ) {
		CHECK(tree.contains(it));
	}

	// Verifies all children have correct parents
	REQUIRE(tree.validate());
}

TEST_CASE( "Erase - sequential", "[erase][shrink]" ) {
	const int numTests = 10;
	tft::TwoFourTree<int> tree;

	for (int i = 0; i < numTests; i++)
		tree.insert(std::move(i));

	SECTION("Remove in order") {
		for (int i = 0; i < numTests; i++) {
			tree.erase(i);
			REQUIRE(!tree.contains(i));
		}
	}

	SECTION("Remove in reverse order") {
		for (int i = numTests - 1; i >= 0; i--) {
			tree.erase(i);
			REQUIRE(!tree.contains(i));
		}
	}

	REQUIRE(tree.validate());
}

/**
 * Stress test
 */
TEST_CASE( "Erase - stress test", "[erase][insert]" ) {

	const int numTests = 50000;

	tft::TwoFourTree<int> tree;

	for (int i = 0; i < numTests; i++)
		tree.insert(std::move(i));

	SECTION("Remove in order") {
		for (int i = 0; i < numTests; i++) {
			tree.erase(i);
			REQUIRE(!tree.contains(i));
		}
	}

	SECTION("Remove in reverse order") {
		for (int i = numTests - 1; i >= 0; i--) {
			tree.erase(i);
			REQUIRE(!tree.contains(i));
		}
	}

	CHECK(tree.validate());
}

