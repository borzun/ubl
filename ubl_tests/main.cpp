#include "ThreadPool.h"
#include "gtest\gtest.h"

#include <iostream>
#include <chrono>

#include "BitonicSort.h"

#include "MatrixSolution.h"

// Typedefs and constants:
using msecs_t = std::chrono::milliseconds;



// Tests:

bool waitForValueChange(std::atomic<bool>& value, 
						const msecs_t waitPeriod = msecs_t(1000),
						const msecs_t check_interval = msecs_t(5)) noexcept
{
	using Type = bool;
	Type previous_value = value;

	auto startLoopTimePoint = std::chrono::high_resolution_clock::now();
	while (true) {
		std::this_thread::sleep_for(check_interval);
		
		if (value != previous_value) {
			return true;
		}

		auto endTimePoint = std::chrono::high_resolution_clock::now();
		auto elapsedTime = (endTimePoint - startLoopTimePoint);
		if (elapsedTime > waitPeriod) {
			break;
		}
	}

	return false;
}

TEST(TestThreadPool, TestThreadPoolRun)
{
	std::vector<std::vector<int>> vec = {
		std::vector<int>({ 1, 1, 0, 0, 1, 0, 0, 1, 1, 0 }),
		std::vector<int>({ 1, 0, 0, 1, 0, 1, 1, 1, 1, 1 }),
		std::vector<int>({ 1, 1, 1, 0, 0, 1, 1, 1, 1, 0 }),
		std::vector<int>({ 0, 1, 1, 1, 0, 1, 1, 1, 1, 1 }),
		std::vector<int>({ 0, 0, 1, 1, 1, 1, 1, 1, 1, 0 }),
		std::vector<int>({ 1, 1, 1, 1, 1, 1, 0, 1, 1, 1 }),
		std::vector<int>({ 0, 1, 1, 1, 1, 1, 1, 0, 0, 1 }),
		std::vector<int>({ 1, 1, 1, 1, 1, 0, 0, 1, 1, 1 }),
		std::vector<int>({ 0, 1, 0, 1, 1, 0, 1, 1, 1, 1 }),
		std::vector<int>({ 1, 1, 1, 0, 1, 0, 1, 1, 1, 1 })
	};

	Solution s;
	s.updateMatrix(vec);
		
	std::unique_ptr<ThreadPool> pool = std::make_unique<ThreadPool>();

	std::atomic<bool> check_value = false;
	pool->run([](std::atomic<bool>& condition)->void
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		condition = true;
	}, 
	std::ref(check_value));

	ASSERT_TRUE(waitForValueChange(check_value));
}

TEST(TestThreadPool, TestDefaultThreadsCount)
{
	std::unique_ptr<ThreadPool> pool = std::make_unique<ThreadPool>();
	ASSERT_EQ(pool->getThreadsCount(), std::thread::hardware_concurrency());
}

TEST(TestThreadPool, TestManualThreadsCount)
{
	std::unique_ptr<ThreadPool> pool = std::make_unique<ThreadPool>(3u);
	ASSERT_EQ(pool->getThreadsCount(), 3);
}


TEST(TestThreadPool, TestIdleFunction)
{
	size_t threadsCount = 3;
	std::unique_ptr<ThreadPool> pool = std::make_unique<ThreadPool>(threadsCount);

	// No job to be done - pool should be Idle:
	ASSERT_TRUE(pool->isIdle());

	for (size_t i = 1; i <= threadsCount; ++i) {
		pool->run([](int index) {
			std::this_thread::sleep_for(msecs_t(index * 100));
		}, i);
	}

	// There are jobs, which need to be completed - pool shouldn't be idle:
	ASSERT_FALSE(pool->isIdle());

	// After half of the work, Pool should be still idle with last job:
	std::this_thread::sleep_for(msecs_t(200));
	ASSERT_FALSE(pool->isIdle());

	// At the end, pool should be idle:
	std::this_thread::sleep_for(msecs_t(200));
	ASSERT_TRUE(pool->isIdle());
}

TEST(TestThreadPool, TestSpuriousThreadPoolDestroy)
{
	size_t threadsCount = 3;
	std::unique_ptr<ThreadPool> pool = std::make_unique<ThreadPool>(threadsCount);

	// No job to be done - pool should be Idle:
	ASSERT_TRUE(pool->isIdle());

	for (size_t i = 1; i <= threadsCount; ++i) {
		pool->run([](int index) {
			std::this_thread::sleep_for(msecs_t(index * 100));
		}, i);
	}

	// There are jobs, which need to be completed - pool shouldn't be idle:
	ASSERT_FALSE(pool->isIdle());
}


int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	
	//::testing::GTEST_FLAG(filter) = "TestVariant.*";
	auto success = RUN_ALL_TESTS();

	std::cout << "Press any key to continue...";
	std::cin.get();

	return success;
}