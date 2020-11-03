#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include <tuple>

#include "catch.hpp"

using namespace std;

void divide(int x, int y, int& quotient_inout, int& reminder_inout)
{
    quotient_inout = x / y;
    reminder_inout = x % y;
}

namespace Cpp17
{
    auto divide(int x, int y)
    {
        return std::tuple(x / y, x % y);
    }
}

TEST_CASE("div")
{
    int x = 5;
    int y = 2;

    int quotient, reminder;
    divide(x, y, quotient, reminder);
    
    cout << quotient << "  " << reminder << endl;

    REQUIRE(quotient == 2);
    REQUIRE(reminder == 1);
}

TEST_CASE("div - c++17")
{
    int x = 5;
    int y = 2;

    auto [quotient, reminder] = Cpp17::divide(x, y); // structured bindings

    REQUIRE(quotient == 2);
    REQUIRE(reminder == 1);
}