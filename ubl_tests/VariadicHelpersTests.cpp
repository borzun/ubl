#include <string>
#include <vector>

#include "utils\VariadicHelpers.h"

#include "gtest\gtest.h"

TEST(TestVariadicHelpers, TestVariadicMaxSizeof)
{
	// test 0 parameters
	ASSERT_EQ(0, ubl::variadic_max_sizeof<>());
	
	// test 1 parameter
	ASSERT_EQ(sizeof(int),				ubl::variadic_max_sizeof<int>());
	ASSERT_EQ(sizeof(double),			ubl::variadic_max_sizeof<double>());
	ASSERT_EQ(sizeof(std::vector<int>), ubl::variadic_max_sizeof<std::vector<int>>());

	// test 2 parameters
	// TODO: for some reason, there is a compile error when do not include call of variadic_max_sizeof into brackets...
	ASSERT_EQ(sizeof(double),	(ubl::variadic_max_sizeof<double, int>()));
	ASSERT_EQ(sizeof(short),	(ubl::variadic_max_sizeof<char, short>()));

	// test 3+ parameters
	ASSERT_EQ(sizeof(double),		(ubl::variadic_max_sizeof<bool, char, short, int, long, double>()));
	ASSERT_EQ(sizeof(double),		(ubl::variadic_max_sizeof<double, long, int, short, char, bool>()));
	ASSERT_EQ(sizeof(std::string),	(ubl::variadic_max_sizeof<int, char, std::string, bool, bool, bool>()));
}

TEST(TestVariadicHelpers, TestIsTypeVariadic)
{
	// test 0 parameters
	ASSERT_FALSE(ubl::is_type_variadic<>());

	// test 1 parameter
	ASSERT_FALSE(ubl::is_type_variadic<int>());
	ASSERT_FALSE(ubl::is_type_variadic<double>());
	ASSERT_FALSE(ubl::is_type_variadic<char>());

	// test 2 parameters
	// TODO: for some reason, there is a compile error when do not include call of variadic_max_sizeof into brackets...
	ASSERT_FALSE((ubl::is_type_variadic<int, double>()));
	ASSERT_FALSE((ubl::is_type_variadic<char, int>()));
	ASSERT_FALSE((ubl::is_type_variadic<short, int>()));
	ASSERT_FALSE((ubl::is_type_variadic<double, int>()));
	ASSERT_FALSE((ubl::is_type_variadic<float, double>()));
	ASSERT_TRUE((ubl::is_type_variadic<double, double>()));
	ASSERT_TRUE((ubl::is_type_variadic<int, int>()));

	// test 3+ parameters
	ASSERT_TRUE((ubl::is_type_variadic<int, double, char, int>()));
	ASSERT_TRUE((ubl::is_type_variadic<char, double, char, int>()));
	ASSERT_TRUE((ubl::is_type_variadic<double, double, char, int>()));
	ASSERT_FALSE((ubl::is_type_variadic<std::string, double, float, int>()));
	ASSERT_TRUE((ubl::is_type_variadic<std::string, double, float, int, short, std::string>()));
	ASSERT_TRUE((ubl::is_type_variadic<int, double, float, int, short, std::string, std::vector<float>>()));
	ASSERT_TRUE((ubl::is_type_variadic<float, double, float, int, short, std::string, std::vector<float>>()));
	ASSERT_TRUE((ubl::is_type_variadic<std::vector<float>, double, float, int, short, std::string, std::vector<float>>()));
	ASSERT_FALSE((ubl::is_type_variadic<std::vector<int>, double, float, int, short, std::string, std::vector<float>>()));
	ASSERT_FALSE((ubl::is_type_variadic<char, double, float, int, short, std::string, std::vector<float>>()));
	ASSERT_FALSE((ubl::is_type_variadic<unsigned, double, float, int, short, std::string, std::vector<float>>()));
}

TEST(TestVariadicHelpers, TestGetTypeIndex)
{
	// test 0 parameters
	ASSERT_EQ(ubl::get_type_index<>(), ubl::ubl_npos);

	// test 1 parameter
	/*ASSERT_EQ(ubl::get_type_index<int>(), ubl::ubl_npos);
	ASSERT_EQ(ubl::get_type_index<std::vector<int>>(), ubl::ubl_npos);*/

	// test 2 parameter
	ASSERT_EQ((ubl::get_type_index<int, int>()), 0);
	ASSERT_EQ((ubl::get_type_index<std::string, std::string>()), 0);
	ASSERT_EQ((ubl::get_type_index<float, int>()), ubl::ubl_npos);

	// test 3 parameter
	ASSERT_EQ((ubl::get_type_index<int, int, float, std::vector<int>>()), 0);
	ASSERT_EQ((ubl::get_type_index<float, int, float, std::vector<int>>()), 1);
	ASSERT_EQ((ubl::get_type_index<std::vector<int>, int, float, std::vector<int>>()), 2);
	ASSERT_EQ((ubl::get_type_index<double, int, float, std::vector<int>>()), ubl::ubl_npos);
	ASSERT_EQ((ubl::get_type_index<char, int, float, std::vector<int>>()), ubl::ubl_npos);

	// test 2 parameters
	// TODO: for some reason, there is a compile error when do not include call of variadic_max_sizeof into brackets...
	

	// test 3+ parameters
	
}

