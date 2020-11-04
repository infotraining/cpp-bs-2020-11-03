#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include "catch.hpp"

using namespace std;

TEST_CASE("const")
{
	const int x = 42;
	const int& cref_x = x;
}

TEST_CASE("const + pointers")
{
	SECTION("pointer to const")
	{
		int x = 42;
		int y = 1024;

		const int* ptr_to_const = &x;

		//*ptr_to_const = 665; // ERROR

		ptr_to_const = &y;

		REQUIRE(*ptr_to_const == 1024);

		const std::string name = "Jan";

		const std::string* ptr = &name;
		const std::string& ref = name;
	}

	SECTION("const pointer")
	{
		int x = 42;
		int y = 1024;

		int* const const_ptr = &x;

		*const_ptr = 665; // OK
		//const_ptr = &y; // ERROR
	}

	SECTION("const pointer to const")
	{
		int x = 42;
		int y = 1024;

		const int* const const_ptr_to_const = &x;

		//*const_ptr_to_const = 665; // ERROR
		//const_ptr_to_const = &y; // ERROR
	}
}
