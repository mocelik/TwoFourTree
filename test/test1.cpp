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

TEST_CASE( "Sample test case", "[tmp]" ) {

    int i = 0;

    REQUIRE( i == 0 );

    SECTION( "first section" ) {
        REQUIRE( i == 0 );
    }
    SECTION( "second section" ) {
        REQUIRE( i == 0 );
        REQUIRE( i == 1 );
    }
}
