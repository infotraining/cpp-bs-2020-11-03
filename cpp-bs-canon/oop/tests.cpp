#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include <list>
#include <execution>
#include <deque>
#include <set>
#include <map>
#include <unordered_map>

#include "catch.hpp"

using namespace std;

class Array
{
private:
	size_t size_;
	int* data_;
public:
	Array(size_t size)
		: size_(size), data_(new int[size])
	{	
		std::fill_n(data_, size_, 0);
	}

	size_t size() const
	{
		return size_;
	}

	int& operator[](size_t index)
	{
		return data_[index];
	}
};

TEST_CASE("Array - construction with size")
{
	Array arr1(10);

	SECTION("size is set")
	{
		REQUIRE(arr1.size() == 10);
	}

	SECTION("all items are zero")
	{
		for (size_t i = 0; i < arr1.size(); ++i)
			CHECK(arr1[i] == 0);
	}
}

TEST_CASE("Indexing")
{
	Array arr1(10);

	arr1[0] = 1;
	arr1[5] = 6;

	CHECK(arr1[0] == 1);
	CHECK(arr1[5] == 6);
}


class MultiArray
{
private:
	size_t size_;
	int* data1_;
	int* data2_;
public:
	MultiArray(size_t size)
		: size_(size), data1_(new int[size]), data2_(new int[size])
	{
		std::fill_n(data1_, size_, 0);
		std::fill_n(data2_, size_, 0);
	}

	size_t size() const
	{
		return size_;
	}

	std::tuple<int&, int&> operator[](size_t index)
	{
		return std::tie(data1_[index], data2_[index]);
	}
};

TEST_CASE("MultiArray")
{
	MultiArray ma(10);

	tuple<int, int> col = ma[0];
	CHECK(std::get<0>(col) == 0);
	CHECK(std::get<1>(col) == 0);

	ma[0] = tuple(1, 2);

	auto [firt_item, second_item] = ma[0];
	CHECK(firt_item == 1);
	CHECK(second_item == 2);
}