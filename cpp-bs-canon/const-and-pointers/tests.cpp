#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include "catch.hpp"

using namespace std;

TEST_CASE("div")
{
    int x = 5;
    int y = 2;

    int quotient, reminder;

    div(x, y, quotient, reminder);

    REQUIRE(quotient == 2);
    REQUIRE(reminder == 1);
}
