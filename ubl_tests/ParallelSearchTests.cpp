#include <vector>
#include <numeric>

#include "utils\tests_helper.h"
#include "ParallelSearch.h"
#include "utils\ElapsedTimer.h"
#include "utils\tests_helper.h"

#include "gtest\gtest.h"

std::vector<int> initLargeVector()
{
	return ubl::tests_helper::generateRandomVector(4000000);
}

TEST(TestParallelSearch, TestParallelSearchSuccess)
{
	std::vector<int> vec = { 1, 5, 9, 2, 4, 7, 8 };

	bool isOk = ubl::parallelSearch(vec.begin(), vec.end(), 5);
	ASSERT_TRUE(isOk);
}

TEST(TestParallelSearch, TestParallelSearchFailure)
{
	std::vector<int> vec = { 1, 5, 9, 2, 4, 7, 8 };

	bool isOk = ubl::parallelSearch(vec.begin(), vec.end(), 11);
	ASSERT_FALSE(isOk);
}


TEST(TestParallelSearch, TestParallelSearchLargeVectorSuccess)
{
	std::vector<int> vec = initLargeVector();
	const int VALUE_TO_FIND = 3997775;

	std::vector<int> values_to_find = { VALUE_TO_FIND };
	for (int i = 0; i < 100; ++i) {
		int value = ubl::tests_helper::createRandomValue(0, vec.size());
		values_to_find.push_back(value);
	}

	long long std_time_ms = 0;
	{
		ubl::DefaultElapsedTimer timer;
		
		int found_values_count = 0;
		for (auto value : values_to_find) {
			auto foundIter = std::find(std::begin(vec), std::end(vec), value);
			if (foundIter != std::end(vec)) {
				++found_values_count;
			}
		}
		
		ASSERT_EQ(found_values_count, values_to_find.size());
		
		std_time_ms = timer.elapsedMsecs().count();
	}

	long long ubl_time_ms = 0;
	{
		ubl::DefaultElapsedTimer timer;

		int found_values_count = 0;
		for (auto value : values_to_find) {
			if (ubl::parallelSearch(std::begin(vec), std::end(vec), value)) {
				++found_values_count;
			}
		}

		ASSERT_EQ(found_values_count, values_to_find.size());
		ubl_time_ms = timer.elapsedMsecs().count();
	}

	std::cout << "UBL_time[ms]: " << ubl_time_ms << "; std_time:[ms]" << std_time_ms << "\n";

	//ASSERT_TRUE(ubl_time_ms <= (std_time_ms * 3 / 2));
}

TEST(TestParallelSearch, TestParallelSearchLargeVectorFailure)
{
	std::vector<int> vec = initLargeVector();

	std::vector<int> values_to_find;
	for (int i = 0; i < 100; ++i) {
		int value = vec.size() + ubl::tests_helper::createRandomValue(0, vec.size());
		values_to_find.push_back(value);
	}

	long long std_time_ms = 0;
	{
		ubl::DefaultElapsedTimer timer;

		int not_found_values_count = 0;
		for (auto value : values_to_find) {
			auto foundIter = std::find(std::begin(vec), std::end(vec), value);
			if (foundIter == std::end(vec)) {
				++not_found_values_count;
			}
		}

		ASSERT_EQ(not_found_values_count, values_to_find.size());

		std_time_ms = timer.elapsedMsecs().count();
	}

	long long ubl_time_ms = 0;
	{
		ubl::DefaultElapsedTimer timer;

		int not_found_values_count = 0;
		for (auto value : values_to_find) {
			if (!ubl::parallelSearch(std::begin(vec), std::end(vec), value)) {
				++not_found_values_count;
			}
		}

		ASSERT_EQ(not_found_values_count, values_to_find.size());
		ubl_time_ms = timer.elapsedMsecs().count();
	}

	std::cout << "UBL_time[ms]: " << ubl_time_ms << "; std_time:[ms]" << std_time_ms << "\n";
}

