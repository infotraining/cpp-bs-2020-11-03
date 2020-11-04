#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include "catch.hpp"

using namespace std;

enum DayOfWeek : uint8_t { mon = 1, tue, wed, thd, fri, sat, sun };

TEST_CASE("enums")
{
	DayOfWeek day = mon;

	int index = thd;

	REQUIRE(index == 4);
}

enum class Coffee : uint8_t { espresso = 1, chemex, v60 };

TEST_CASE("scoped enumerations")
{
	Coffee c = Coffee::chemex;

	int index = static_cast<int>(c);

	REQUIRE(index == 2);
}
