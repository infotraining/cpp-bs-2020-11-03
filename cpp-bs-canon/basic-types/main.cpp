#include <iostream>
#include <string>
#include <cassert>

using namespace std::literals;

int main()
{
	char c = -65;
	unsigned char uc{ 65 };

	std::cout << static_cast<int>(uc) << "\n";

	short s = 1024;
	unsigned short us{ 887 };

	int x = 42;
	unsigned int ux = 665;

	long l{ 7'656'576 };
	unsigned long ul = 1'000'000'888UL;

	int8_t x1 = 66;
	uint8_t x2 = 87;

	int16_t x3 = 76;
	int32_t x4 = 665;
	uint64_t x5 = 1'000'000ULL;

	// auto + number literal
	auto ax1 = 42; // int ax1
	auto ax2 = 'a'; // char ax2
	auto ax3 = 42L; // long ax3
	auto ax4 = 1'000'000ULL; // unsigned long long

	// floating points
	float pi_f = 3.14F;
	double pi_d = 3.14;
	auto pi = 3.14;

	// bool
	bool flag = true;
	flag = false;

	flag = s; // s = true - implicit conversion from integral type to bool

	// c-string
	const char* str1 = "text";
	auto astr1 = "text"; // const char*
	assert(strlen(str1) == 4);
	// str1[0] = 'T'; // illegal

	const char* another = "Text";
	assert(strcmp(str1, another) == 1);

	// std::string
	std::string str2 = "text";
	auto astr2 = "text"s;
	str2[0] = 'T';

	assert(str2.size() == 4);
	assert(str2 == "Text"s);
	assert("text"s > str2);

	str2 += "!!!";

	std::cout << str2 << "\n";
}