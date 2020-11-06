#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>
#include <tuple>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <boost/tokenizer.hpp>

#include "catch.hpp"

using namespace std;

TEST_CASE("streams - formatted output")
{
	int a = 10;
	double dbl = 3.14;

	std::cout << setw(10) << setfill('_') << a << "\n";
	std::cout << left << setw(10) << setfill('_') << dbl << "\n";
}

TEST_CASE("streams - formatted input")
{
	std::string input = "14 2.133";

	stringstream in(input);

	int a;
	double b;

	in >> a >> b;

	std::cout << "read: " << a << " " << b << std::endl;
	
	std::cout << "read: " << a << " " << b << "\n";
	std::cout.flush();


	REQUIRE(in);
}

TEST_CASE("binary streams")
{
	const string file_name = "data.bin";

	SECTION("write in binary format")
	{
		ofstream fout(file_name, ios::out | ios::binary);

		if (!fout.is_open())
		{
			cerr << "Error: opening file failed...\n";
			abort();
		}

		int x = 42;
		double pi = 3.14;
		int tab[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

		fout.write(reinterpret_cast<char*>(&x), sizeof(x));
		fout.write(reinterpret_cast<char*>(&pi), sizeof(pi));
		fout.write(reinterpret_cast<char*>(tab), sizeof(tab));

		fout.close();
	}

	SECTION("read in binary format")
	{
		ifstream fin(file_name, ios::in | ios::binary);

		if (!fin.is_open())
		{
			cerr << "Error: opening file failed...\n";
			abort();
		}

		int x;
		double pi;
		int tab[10];

		fin.read(reinterpret_cast<char*>(&x), sizeof(int));
		fin.read(reinterpret_cast<char*>(&pi), sizeof(double));
		fin.read(reinterpret_cast<char*>(tab), sizeof(tab));

		REQUIRE(x == 42);
		REQUIRE(pi == Approx(3.14));

		int expected[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

		REQUIRE(std::equal(std::begin(tab), std::end(tab), std::begin(expected)));

		fin.close();
	}
}

struct Data
{
	std::vector<int> data;
};

namespace DataIO
{
	void read(istream& in, Data& d)
	{
		size_t size;
		in.read(reinterpret_cast<char*>(&size), sizeof(size_t));
		d.data.resize(size);
		in.read(reinterpret_cast<char*>(d.data.data()), size * sizeof(int));
	}

	void write(ostream& out, const Data& d)
	{
		size_t size = d.data.size();
		out.write(reinterpret_cast<char*>(&size), sizeof(size_t));
		out.write(reinterpret_cast<const char*>(d.data.data()), d.data.size() * sizeof(int));
	}
};

TEST_CASE("i/o for objects")
{
	Data row{ { 1, 2, 3, 4, 5 } };

	const string file_name = "data.bin";

	SECTION("write in binary format")
	{
		ofstream fout(file_name, ios::out | ios::binary);

		if (!fout.is_open())
		{
			cerr << "Error: opening file failed...\n";
			abort();
		}

		DataIO::write(fout, row);

		fout.close();
	}

	SECTION("read in binary format")
	{
		ifstream fin(file_name, ios::in | ios::binary);

		if (!fin.is_open())
		{
			cerr << "Error: opening file failed...\n";
			abort();
		}

		Data expected;

		DataIO::read(fin, expected);

		REQUIRE(expected.data == row.data);
		
		fin.close();
	}

}

TEST_CASE("boost tokenizer")
{
	std::string str = ";;Hello|world||-foo--bar;yow;baz|";

	using tokenizer = boost::tokenizer<boost::char_separator<char>>;

	boost::char_separator<char> sep("-;|");
	tokenizer tokens(str, sep);

	for (const auto& token : tokens)
		std::cout << token << " ";
	std::cout << "\n";
}