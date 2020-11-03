#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include <array>
#include <vector>
#include <algorithm>
#include <numeric>

namespace CStyle
{
	void fill_tab(int* tab, size_t size)
	{
		int value = 0;
		for (int* it = tab; it != tab + size; ++it)
			*it = ++value;
	}
}

namespace Step1
{
	template <typename T, size_t N>
	void fill_tab(T(&tab)[N])
	{
		T value{};
		for (T& item : tab)
			item = ++value;
	}
}

template <typename TContainer>
void fill_container(TContainer& container)
{
	using T = std::decay_t<decltype(container[0])>;
	
	T value{};

	for (T& item : container)
		item = ++value;
}

template <typename T>
void print(const T& container)
{
	for (auto item : container)
		std::cout << item << " ";
	std::cout << "\n";
}


TEST_CASE("native arrays - old style")
{
	constexpr int size = 20;
	int tab1[size] = {1, 2, 3};

	tab1[5] = 6;

	//CStyle::fill_tab(tab1, 10);
	std::iota(std::begin(tab1), std::end(tab1), 1);
	print(tab1);

	double dbls[10];

	//fill_container(dbls);
	std::fill(std::begin(dbls), std::end(dbls), 3.14);
	print(dbls);

	std::string text = "text";
	print(text);
}

TEST_CASE("std::array - modern")
{
	std::array<int, 10> arr1 = { 1, 2, 3 };

	arr1[5] = 6;

	print(arr1);

	REQUIRE(arr1.size() == 10);

	arr1.fill(-1);
}

TEST_CASE("dynamic arrays - old style")
{
	int size = 10;
	int* tab = new int[size];

	for (int i = 0; i < size; ++i)
		tab[i] = i;

	delete[] tab;
}

TEST_CASE("vector - modern")
{
	std::vector<int> vec(10);

	vec.resize(30);

	fill_container(vec);

	print(vec);
}

TEST_CASE("multidimensional vector")
{
	using Matrix = std::vector<std::vector<int>>;

	Matrix matrix = { {1, 2, 3}, {4, 5, 6}, {7, 8, 9} };

	matrix[1][1] = 0;

	REQUIRE(matrix[1].size() == 3);

	for (const auto& row : matrix)
	{
		for (const auto& item : row)
		{
			std::cout << item << " ";
		}
		std::cout << "\n";
	}
}