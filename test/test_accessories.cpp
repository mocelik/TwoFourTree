/*
 *  Filename:   test_accessories.cpp
 *
 *  Created on: Jun 20, 2020
 *      Author: orhan
 *
 *  Description:
 * 		Tests the simpler methods of the tree, such as:
 * 			count
 * 			clear
 * 			contains
 * 			  etc..
 */

#include "../src/tftree.hpp"
#include "catch.hpp"

#include "test_utils.hpp"

/**
 * contains should return true for items in the tree and false otherwise
 */
TEST_CASE( "Accessory: contains" ) {

	tft::TwoFourTree<int> tree;

	auto values = generateUnique(100);

	for (const auto& it : values)
		tree.insert(std::move(int(it)));

	for (const auto& it : values)
		CHECK(tree.contains(it));

	auto missingValues = generateUnique(100, values);

	for (const auto& it : missingValues)
		CHECK(!tree.contains(it));
}

TEST_CASE( "Accessory: contains template", "[.todo]" ) {

	tft::TwoFourTree<int> tree;

	auto values = generateUnique(1);

	for (const auto& it : values)
		tree.insert(std::move(int(it)));

	for (const auto& it : values)
		CHECK(tree.contains<long>(static_cast<long>(it)));

	auto missingValues = generateUnique(1, values);

	for (const auto& it : missingValues)
		CHECK(!tree.contains<long>(static_cast<long>(it)));
}

/**
 * count should return the correct number of items in the tree
 */
TEST_CASE("Accessory: count") {
	auto values = generateUnique(100);

	tft::TwoFourTree<int> tree;
	for (const auto& it : values)
		tree.insert(std::move(int(it)));

	for (const auto& it : values)
		CHECK(tree.count(it) == 1);

	auto missingValues = generateUnique(100, values);
	for (const auto& it : missingValues)
		CHECK(tree.count(it) == 0);
}

TEST_CASE("Accessory: count template", "[.todo]") {
	auto values = generateUnique(1);

	tft::TwoFourTree<int> tree;
	for (const auto& it : values)
		tree.insert(std::move(int(it)));

	for (const auto& it : values)
		CHECK(tree.count<long>(static_cast<long>(it)) == 1);

	auto missingValues = generateUnique(1, values);
	for (const auto& it : missingValues)
		CHECK(tree.count<long>(static_cast<long>(it)) == 0);
}

/**
 * find should return an interator to the element if exists, end iterator otherwise
 */
TEST_CASE("Accessory: find") {
	auto values = generateUnique(100);

	tft::TwoFourTree<int> tree;
	for (const auto& it : values)
		tree.insert(std::move(int(it)));

	for (const auto& it : values)
		CHECK(*tree.find(it) == it);

	auto missingValues = generateUnique(100, values);
	for (const auto& it : missingValues)
		CHECK(tree.find(it) == tree.end());
}

/**
 * empty should return true if empty, false otherwise
 */
TEST_CASE("Accessory: empty") {
	auto values = generateUnique(100);

	tft::TwoFourTree<int> tree;
	CHECK(tree.empty());
	tree.insert(std::move(1));
	CHECK(!tree.empty());
	tree.erase(1);
	CHECK(tree.empty());
}

/**
 * empty should return true if empty, false otherwise
 */
TEST_CASE("Accessory: size") {
	auto values = generateUnique(100);
	tft::TwoFourTree<int>::size_type size = 0;

	tft::TwoFourTree<int> tree;
	for (const auto &it : values) {
		tree.insert(int(it));
		CHECK(tree.size() == ++size);
	}

	for (const auto& it : values) {
		tree.erase(it);
		CHECK(tree.size() == --size);
	}
}

