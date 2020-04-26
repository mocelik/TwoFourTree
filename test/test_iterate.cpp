/*
 *  Filename:   test1.cpp
 *
 *  Created on: Mar. 22, 2020
 *      Author: orhan
 *
 *  Description:
 * 		Temporary file to be updated with some test cases
 */

#include "../src/tftree.hpp"
#include "catch.hpp"

#include <cstdlib>
#include <vector>
#include <algorithm>
#include <numeric>

/**
 * Used tree:
 *     [ 1, 2, 3 ]
 */
TEST_CASE( "Simple Iterate", "[iterate][insert]" ) {

	tft::TwoFourTree<int> tree;

	// first few values will go into root
	tft::TwoFourTree<int>::iterator rc_check;
	rc_check = tree.insert(1).first;
	CHECK(*rc_check == 1);

	rc_check = tree.insert(2).first;
	CHECK(*rc_check == 2);

	rc_check = tree.insert(3).first;
	CHECK(*rc_check == 3);

	{
		tft::TwoFourTree<int>::iterator it = tree.begin();

		CHECK(*it == 1);
		++it;
		CHECK(*it == 2);
		++it;
		CHECK(*it == 3);

		++it;
		CHECK(it == tree.end());
	}

	{ // forward it -> operator--
		tft::TwoFourTree<int>::iterator it_negative = tree.end();
		--it_negative;
		CHECK(*it_negative == 3);
		CHECK(*--it_negative == 2);
		CHECK(*--it_negative == 1);
		CHECK(it_negative == tree.begin());
	}


	{ // reverse it -> operator++
		tft::TwoFourTree<int>::reverse_iterator rit = tree.rbegin();
		CHECK(*rit == 3);

		++rit;
		CHECK(*rit == 2);

		++rit;
		CHECK(*rit == 1);

		++rit;
		CHECK(rit == tree.rend());
	}
}

/**
 * Used tree:
 * 			[ 2 ]
 * 		[ 1 ]   [ 3, 4 ]
 */
TEST_CASE ("Iterate to neighbour", "[iterate][insert]") {
	tft::TwoFourTree<int> tree;

	// first few values will go into root
	tree.insert(1);
	tree.insert(2);
	tree.insert(3);
	tree.insert(4);

	tft::TwoFourTree<int>::iterator it = tree.begin();
	REQUIRE(*it == 1);
	CHECK(*++it == 2);
	CHECK(*++it == 3);
	CHECK(*++it == 4);

	CHECK(++it == tree.end());
}

/**
 * Same test as above but with the parent containing multiple keys and children
 */
TEST_CASE ("Iterate to multiple neighbours", "[iterate][insert]") {

	const int num_elems = 8;
	tft::TwoFourTree<int> tree;


	for ( int i=0; i < num_elems; i++) {
		tree.insert(std::move(i));
	}

	{
		tft::TwoFourTree<int>::const_iterator it = tree.begin();
		for (int i = 0; i < num_elems; i++) {
			CHECK(*it == i);
			++it;
		}
		CHECK(it == tree.end());
	}

	{
		tft::TwoFourTree<int>::const_reverse_iterator rit = tree.rbegin();
		for (int i = num_elems - 1; i >= 0; i--) {
			CHECK(*rit == i);
			++rit;
		}
		CHECK(rit == tree.rend());
	}
}

/**
 * Stress test
 */
TEST_CASE( "Iterate to multiple neighbours and parents", "[iterate][insert]" ) {

	const int num_tests = 50000;

	tft::TwoFourTree<int> tree;

	SECTION("Add in order") {
		for (int i = 0; i < num_tests; i++) {
			tft::TwoFourTree<int>::const_iterator it = tree.insert(std::move(i)).first;
			CHECK(*it == i);
		}
	}

	SECTION("Add in reverse order") {
		for (int i = num_tests - 1; i >= 0; i--) {
			tft::TwoFourTree<int>::const_iterator it = tree.insert(std::move(i)).first;
			CHECK(*it == i);
		}
	}

	{ // iterate
		tft::TwoFourTree<int>::const_iterator it = tree.begin();
		for (int i = 0; i < num_tests; i++) {
			CHECK(*it == i);
			++it;
		}
		CHECK(it == tree.end());
	}

	{ // iterate backwards
		tft::TwoFourTree<int>::const_iterator it = tree.end();
		for (int i = num_tests-1; i >= 0; i--) {
			CHECK(*--it == i);
		}
		CHECK(it == tree.begin());
	}

	{ // reverse iterate
		tft::TwoFourTree<int>::const_reverse_iterator rit = tree.rbegin();
		for (int i = num_tests-1; i >= 0; i--) {
			CHECK(*rit == i);
			++rit;
		}
		CHECK(rit == tree.rend());
	}

	{
		int i = 0;
		for (const auto& it : tree)
			CHECK(it == i++);
	}
}
