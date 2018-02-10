#include "gtest\gtest.h"

#include "Variant.h"
#include "utils\ElapsedTimer.h"
#include "utils\tests_helper.h"

using namespace ubl::tests_helper;

TEST(TestVariant, TestSetNonCV_Values)
{
	ubl::Variant<int, double, char> variant;

	ASSERT_TRUE(wait_for_exception<ubl::InvalidVariantException>([&variant]()
	{
		variant = 42.f;
	}));
	ASSERT_TRUE(wait_for_exception<ubl::InvalidVariantException>([&variant]()
	{
		variant = std::string("test value");
	}));
	ASSERT_TRUE(wait_for_exception<ubl::InvalidVariantException>([&variant]()
	{
		variant = std::vector<int>();
	}));
}

TEST(TestVariant, TestGetNonCV_Values)
{
	ubl::Variant<int, double, char> variant;

	// Test for empty variant:
	ASSERT_TRUE(wait_for_exception<ubl::InvalidVariantException>([&variant]()
	{
		int i_value = variant.getValue<int>();
		double d_value = variant.getValue<double>();
		char ch_value = variant.getValue<char>();

		FAIL() << "The InvalidVariantException should have been thrown in case when Variant is not valid!";
	}));

	// Tests for int:
	variant = 45;
	ASSERT_EQ(45, variant.getValue<int>());

	try {
		char c_value = variant.getValue<char>();
		short sh_value = variant.getValue<short>();
		unsigned u_value = variant.getValue<unsigned>();
		long l_value = variant.getValue<long>();
		
		FAIL() << "The InvalidVariantException should have been thrown - Variant is type of int!";
	}
	catch (const ubl::InvalidVariantException& exc)
	{
		ASSERT_TRUE(exc.what() != nullptr);
	}

	// Test for double:
	variant = -5.;
	ASSERT_DOUBLE_EQ(-5., variant.getValue<double>());

	try {
		float f_value = variant.getValue<float>();
		int i_value = variant.getValue<int>();

		FAIL() << "The InvalidVariantException should have been thrown - Variant is type of double!";
	}
	catch (const ubl::InvalidVariantException& exc)
	{
		ASSERT_TRUE(exc.what() != nullptr);
	}

	// Test for char:
	variant = 'a';
	ASSERT_DOUBLE_EQ(variant.getValue<char>(), 'a');

	try {
		unsigned char uchar_value = variant.getValue<unsigned char>();
		short short_value = variant.getValue<short>();
		int i_value = variant.getValue<int>();
		double d_value = variant.getValue<double>();

		FAIL() << "The InvalidVariantException should have been thrown - Variant is type of char!";
	}
	catch (const ubl::InvalidVariantException& exc)
	{
		ASSERT_TRUE(exc.what() != nullptr);
	}
}

TEST(TestVariant, TestOnMemoryLeaks)
{
	ubl::DefaultElapsedTimer timer;

	ubl::Variant<int, double, char> variant;
	variant = 42;

	const size_t oldProcessRamMemory = ubl::tests_helper::getCurrentProcessRamMemoryUsage();

	for (int i = 0, currentMode = 0; i < 30000; ++i, currentMode = i % 3)
	{
		switch (currentMode)
		{
			case 0: // int
			{
				const int i_value = rand() % 10000;
				variant = i_value;

				ASSERT_EQ(i_value, variant.getValue<int>());
			}
			break;
			case 1: // double
			{
				const double d_value = (rand() % 10000) * 1.0;
				variant = d_value;

				ASSERT_EQ(d_value, variant.getValue<double>());
			}
			break;
			case 2: // char
			{
				const char ch_value = rand() % std::numeric_limits<char>::max();
				variant = ch_value;

				ASSERT_EQ(ch_value, variant.getValue<char>());
			}
			break;
		}
	}

	const size_t newProcessMemory = ubl::tests_helper::getCurrentProcessRamMemoryUsage();
	ASSERT_EQ(newProcessMemory, oldProcessRamMemory);
}

TEST(TestVariant, TestGetConstVolatileValues)
{
	ubl::Variant<int, double, char> variant;

	// Tests for int:
	const int i_value = 42;
	variant = i_value;
	
	try {
		ASSERT_EQ(i_value, variant.getValue<int>());
		ASSERT_EQ(i_value, variant.getValue<const int>());
		ASSERT_EQ(i_value, variant.getValue<const volatile int>());
	}
	catch (const ubl::InvalidVariantException& exc)
	{
		FAIL() << "ERROR: " << exc.what() << "; all getters with cv and non-cv types should be intercompatibe!";
	}

	// Test for double:
	constexpr double d_value = -42.0;
	variant = d_value;

	try {
		ASSERT_DOUBLE_EQ(d_value, variant.getValue<double>());
		ASSERT_DOUBLE_EQ(d_value, variant.getValue<const double>());
		ASSERT_DOUBLE_EQ(d_value, variant.getValue<const volatile double>());
	}
	catch (const ubl::InvalidVariantException& exc)
	{
		FAIL() << "ERROR: " << exc.what() << "; all getters with cv and non-cv types should be intercompatibe!";
	}
}

TEST(TestVariant, TestChangeStoredValue)
{
	ubl::Variant<int, float> variant;

	// Tests for int:
	variant = 42;
	int& value = variant.getValue<int>();
	value = -42;
	ASSERT_EQ(-42, variant.getValue<int>());

	// Tests for double:
	variant = 52.0f;
	float& f_value = variant.getValue<float>();
	f_value = 42.0f;
	ASSERT_FLOAT_EQ(42.0f, variant.getValue<float>());
}

TEST(TestVariant, TestUserDefinedType)
{
	bool is_user_struct_deleted = false;
	
	struct UserStruct
	{
		UserStruct(double value, std::function<void()> deleterFunc)
			: m_value(value)
			, m_deleter_func(deleterFunc)
		{	}

		~UserStruct() {
			m_deleter_func();
		}

		double m_value;
		std::function<void()> m_deleter_func;
	};

	ubl::Variant<int, UserStruct> variant;

	const double double_value = 42.0;
	UserStruct user_struct = { double_value, [&is_user_struct_deleted]() { is_user_struct_deleted = true; } };
	variant = user_struct;

	try {
		ASSERT_DOUBLE_EQ(double_value, variant.getValue<UserStruct>().m_value);
		ASSERT_DOUBLE_EQ(double_value, variant.getValue<const UserStruct>().m_value);
		ASSERT_DOUBLE_EQ(double_value, variant.getValue<const volatile UserStruct>().m_value);
	}
	catch (const ubl::InvalidVariantException& exc)
	{
		FAIL() << "ERROR: " << exc.what() << "; getters should work correctly for user defined class!";
	}

	// Check whether destructor of user-defined class was correctly called:
	variant = -1;
	ASSERT_EQ(-1, variant.getValue<int>());
	ASSERT_TRUE(is_user_struct_deleted);
}