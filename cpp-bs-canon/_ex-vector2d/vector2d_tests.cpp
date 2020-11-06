#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "catch.hpp"

using namespace std;

namespace Vectors
{
	class Vector2D
	{
		double x_;
		double y_;

		static const Vector2D unit_x_;
		static const Vector2D unit_y_;

	public:
		explicit Vector2D(double x = 0.0, double y = 0.0)
			: x_(x), y_(y)
		{
		}

		double x() const { return x_; }

		double y() const { return y_; }

		double length() const { return sqrt(x_ * x_ + y_ * y_); }
		
		static const Vector2D& unit_x()
		{
			return unit_x_;

		}

		static const Vector2D& unit_y()
		{
			return unit_y_;

		}
	};

	const Vector2D Vector2D::unit_x_(1.0, 0.0);
	const Vector2D Vector2D::unit_y_(0.0, 1.0);

	Vector2D operator+(const Vector2D& v1, const Vector2D& v2)
	{
		return Vector2D(v1.x() + v2.x(), v1.y() + v2.y());
	}
}

using namespace Vectors;

TEST_CASE("vector2D")
{
	SECTION("default constructor")
	{
		Vectors::Vector2D vec;

		REQUIRE(vec.x() == Approx(0.0));
		REQUIRE(vec.y() == Approx(0.0));
	}

	SECTION("constructor with one argument")
	{
		Vectors::Vector2D vec(3.0);

		REQUIRE(vec.x() == 3.0);
		REQUIRE(vec.y() == 0.0);

		Vectors::Vector2D vec2(4.5);
	}

	SECTION("constructor")
	{
		const Vector2D vec(1.0, 2.0);

		REQUIRE(vec.x() == Approx(1.0));
		REQUIRE(vec.y() == Approx(2.0));
	}

	SECTION("length")
	{
		Vector2D vec(1.0, 1.0);

		REQUIRE(vec.length() == Approx(1.41).margin(0.01));
	}

	SECTION("unit vector x")
	{
		Vector2D vx = Vector2D::unit_x(); // static method

		REQUIRE(vx.x() == Approx(1.0));
		REQUIRE(vx.y() == Approx(0.0));
	}

	SECTION("unit vector y")
	{
		Vector2D vy = Vector2D::unit_y();

		REQUIRE(vy.x() == Approx(0.0));
		REQUIRE(vy.y() == Approx(1.0));
	}

	SECTION("operators")
	{

		const Vector2D vec1{ 1.0, 2.0 };
		const Vector2D vec2{ 2.0, 0.5 };

		SECTION("operator +")
		{
			auto vec = vec1 + vec2;

			REQUIRE(vec.x() == 3.0);
			REQUIRE(vec.y() == 2.5);
		}

		SECTION("equality")
		{
			REQUIRE(vec1 == Vector2D{ 1.0, 2.0 });
		}

		SECTION("nonequality")
		{
			REQUIRE(vec1 != vec2);
		}

		SECTION("subtraction")
		{
			auto vec = vec1 - vec2;

			REQUIRE(vec == Vector2D{ -1.0, 1.5 });
		}

		SECTION("negate")
		{
			auto vec = -vec1;

			REQUIRE(vec == Vector2D{ -1.0, -2.0 });
		}

		SECTION("multiplication")
		{
			double scalar = vec1 * vec2;

			REQUIRE(scalar == Approx(3.0));
		}

		SECTION("multiplication by double")
		{
			SECTION("a * Vector2D")
			{
				Vector2D vec = 2.0 * vec1;

				REQUIRE(vec == Vector2D{ 2.0, 4.0 });
			}

			SECTION("Vector2D * a")
			{
				Vector2D vec = vec1 * 2.0;

				REQUIRE(vec == Vector2D{ 2.0, 4.0 });
			}
		}
	}

	//        SECTION("operator <<")
	//        {
	//            cout << "vec1: " << vec1 << endl;

	//            stringstream ss;

	//            ss << vec1;

	//            REQUIRE(ss.str() == "Vector2D(1.0, 2.0)");
	//        }
	//    }
}
