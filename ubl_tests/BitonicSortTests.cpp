#include <cmath>
#include <vector>

#include "BitonicSort.h"
#include "utils/ElapsedTimer.h"
#include "utils/tests_helper.h"

#include "gtest/gtest.h"

TEST(TestBitonicSort, TestBitonicMergeStepForward)
{
	std::vector<int> vec = { 3, 4, 7, 8, 6, 5, 2, 1 };

	auto iterBegin = vec.begin();
	auto iterEnd = vec.end();
	ubl::detail::BitonicSortImpl<decltype(iterBegin)> sorter(iterBegin, iterEnd);
	sorter.bitonicMergeStep(iterBegin, vec.size() / 2, true);

	std::vector<int> expected_vec = { 3, 4, 2, 1, 6, 5, 7, 8 };
	ASSERT_EQ(expected_vec, vec);
}

TEST(TestBitonicSort, TestBitonicMergeStepBackward)
{
	std::vector<int> vec = { 2, 6, 5, 1 };

	auto iterBegin = vec.begin();
	auto iterEnd = vec.end();
	ubl::detail::BitonicSortImpl<decltype(iterBegin)> sorter(iterBegin, iterEnd);
	sorter.bitonicMergeStep(iterBegin, vec.size() / 2, false);

	std::vector<int> expected_vec = { 5, 6, 2, 1 };
	ASSERT_EQ(expected_vec, vec);
}

TEST(TestBitonicSort, TestBitonicMerge)
{
	std::vector<int> vec = { 3, 4, 7, 8, 6, 5, 2, 1 };

	auto iterBegin = vec.begin();
	auto iterEnd = vec.end();
	ubl::detail::BitonicSortImpl<decltype(iterBegin)> sorter(iterBegin, iterEnd);
	sorter.bitonicMerge(iterBegin, vec.size(), true);

	std::vector<int> expected_vec = { 1, 2, 3, 4, 5, 6, 7, 8 };
	ASSERT_EQ(expected_vec, vec);
}

TEST(TestBitonicSort, TestBitonicSort)
{
	std::vector<int> vec = { 3, 7, 4, 8, 6, 2, 1, 5 };
	ubl::bitonicSort(vec);
	
	std::vector<int> expected_vec = { 1, 2, 3, 4, 5, 6, 7, 8 };
	ASSERT_EQ(expected_vec, vec);
}

TEST(TestBitonicSort, TestEvenNumbersSortCorrectness)
{
	// Create input:
	for (auto i = 1; i < 8; ++i) {
		size_t size = std::pow(2, i);
		std::vector<int> vec = ubl::tests_helper::generateRandomVector(size);

		// Etalon (sorted elemenents):
		std::vector<int> sorted_vec = vec;
		std::sort(sorted_vec.begin(), sorted_vec.end());

		// Bitonic sort:
		ubl::bitonicSort(vec);

		// check on correctness:
		ASSERT_EQ(sorted_vec, vec);
	}
}

TEST(TestBitonicSort, TestSpeed)
{
	std::vector<int> vec = ubl::tests_helper::generateVeryLargeRandomVector();

	// test bitonic sort
	std::vector<int> bitonic_sorted = vec;
	std::chrono::milliseconds elapsedTime_bitonic;
	{
		ubl::DefaultElapsedTimer timer;
		ubl::bitonicSort(bitonic_sorted);
		elapsedTime_bitonic = timer.elapsedMsecs();
	}

	// test simple quick sort:
	std::chrono::milliseconds elapsedTime_stl;
	std::vector<int> stl_sorted = vec;
	{
		ubl::DefaultElapsedTimer timer;

		std::sort(std::begin(stl_sorted), std::end(stl_sorted));

		elapsedTime_stl = timer.elapsedMsecs();
	}

	ASSERT_EQ(stl_sorted, bitonic_sorted);

	auto bitonic_sort_msecs = elapsedTime_bitonic.count();
	auto stl_sort_msecs = elapsedTime_stl.count();
	std::cout << "bitonic_sort: " << bitonic_sort_msecs << " msecs; stl_sort = " << stl_sort_msecs << " msecs \n";

	ASSERT_TRUE(bitonic_sort_msecs <= stl_sort_msecs * 3);
}
