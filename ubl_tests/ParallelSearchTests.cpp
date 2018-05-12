#include <vector>
#include <numeric>

#include "utils\tests_helper.h"
#include "ParallelSearch.h"
#include "utils\ElapsedTimer.h"
#include "utils\tests_helper.h"

#include "gtest\gtest.h"

static const std::vector<int> s_largeVector = ubl::tests_helper::generateRandomVector(4000000);

TEST(TestParallelSearch, TestParallelSearchAsyncVector)
{
	const std::vector<int>& vec = ubl::tests_helper::generateRandomVector(5000);

	std::vector<int> values_to_find;
	for (int i = 0; i < 100; ++i) {
		int value = ubl::tests_helper::createRandomValue(0, vec.size() * 2);
		values_to_find.push_back(value);
	}

	size_t std_found_values_count = 0;
	{
		for (auto value : values_to_find) {
			auto foundIter = std::find(std::begin(vec), std::end(vec), value);
			if (foundIter != std::end(vec)) {
				++std_found_values_count;
			}
		}
	}

	size_t ubl_found_values_count = 0;
	{
		for (auto value : values_to_find) {
			if (ubl::parallelSearch(std::begin(vec), std::end(vec), value, ubl::ParallelSearchImplementation::Async)) {
				++ubl_found_values_count;
			}
		}
	}

	ASSERT_EQ(std_found_values_count, ubl_found_values_count);
}

TEST(TestParallelSearch, TestParallelSearchOpenMPVector)
{
	const std::vector<int>& vec = ubl::tests_helper::generateRandomVector(5000);

	std::vector<int> values_to_find;
	for (int i = 0; i < 100; ++i) {
		int value = ubl::tests_helper::createRandomValue(0, vec.size() * 2);
		values_to_find.push_back(value);
	}

	size_t std_found_values_count = 0;
	{
		for (auto value : values_to_find) {
			auto foundIter = std::find(std::begin(vec), std::end(vec), value);
			if (foundIter != std::end(vec)) {
				++std_found_values_count;
			}
		}
	}

	size_t ubl_found_values_count = 0;
	{
		for (auto value : values_to_find) {
			if (ubl::parallelSearch(std::begin(vec), std::end(vec), value, ubl::ParallelSearchImplementation::Async)) {
				++ubl_found_values_count;
			}
		}
	}

	ASSERT_EQ(std_found_values_count, ubl_found_values_count);
}

TEST(TestParallelSearch, TestParallelSearchAsyncLargeVectorSuccess)
{
	const std::vector<int>& vec = s_largeVector;

	std::vector<int> values_to_find;
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
			if (ubl::parallelSearch(std::begin(vec), std::end(vec), value, ubl::ParallelSearchImplementation::Async)) {
				++found_values_count;
			}
		}

		ASSERT_EQ(found_values_count, values_to_find.size());
		ubl_time_ms = timer.elapsedMsecs().count();
	}

	std::cout << "UBL_time[ms]: " << ubl_time_ms << "; std_time:[ms]" << std_time_ms << "\n";

	//ASSERT_TRUE(ubl_time_ms <= (std_time_ms * 3 / 2));
}

TEST(TestParallelSearch, TestParallelSearchAsyncLargeVectorFailure)
{
	const std::vector<int>& vec = s_largeVector;

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
			if (!ubl::parallelSearch(std::begin(vec), std::end(vec), value, ubl::ParallelSearchImplementation::Async)) {
				++not_found_values_count;
			}
		}

		ASSERT_EQ(not_found_values_count, values_to_find.size());
		ubl_time_ms = timer.elapsedMsecs().count();
	}

	std::cout << "UBL_time[ms]: " << ubl_time_ms << "; std_time:[ms]" << std_time_ms << "\n";
}

