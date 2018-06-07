#pragma once

#include <ubl.h>

#include <future>
#include <thread>
#include <condition_variable>

UBL_NAMESPACE_BEGIN

DETAIL_NAMESPACE_BEGIN

// The utility class for Master-Slave multithreading patterns for dividing the work between slaves
template <class IterType>
class MasterSlavePattern
{
public:

	using FuncType = std::function<void(IterType, IterType)>;
	using FinishFuncType = std::function<bool()>;

	MasterSlavePattern(IterType begin, IterType end, 
					   const FuncType& evalFunc, 
					   const FinishFuncType& finishFunc = ALWAYS_FALSE_FUNC)
		: m_evalFunc(evalFunc)
		, m_finishFunc(finishFunc)
	{
		const auto SIZE = std::distance(begin, end);
		const auto bucket_size = SIZE / THREADS_COUNT;

		auto curr_iter = begin;

		for (size_t i = 0; i < THREADS_COUNT; ++i) {
			const auto next_size = (i < THREADS_COUNT - 1) ? bucket_size
				: (SIZE - (bucket_size * (THREADS_COUNT - 1)));

			auto next_iter = std::next(curr_iter, next_size);
			auto handle = std::async(std::launch::async, 
									 &MasterSlavePattern<IterType>::performSlaveFunc, 
									 this, curr_iter, next_iter);

			curr_iter = next_iter;
			curr_iter = next_iter;
		}

		std::unique_lock<std::mutex> guard(m_mutex);
		m_conditional.wait(guard, [this]() { return areThreadSlavesFinished(); });
	}

	MasterSlavePattern(const MasterSlavePattern& other) = delete;
	MasterSlavePattern& operator=(const MasterSlavePattern& other) = delete;

private: // Methods

	void performSlaveFunc(IterType begin, IterType end)
	{
		m_evalFunc(begin, end);

		{
			std::unique_lock<std::mutex> lock(m_mutex);
			--m_threadsCounter;
			if (areThreadSlavesFinished()) {
				m_conditional.notify_one();
			}
		}
	}

	inline bool areThreadSlavesFinished() const
	{
		return m_threadsCounter == 0 || m_finishFunc();
	}


private: // Members
	
	const size_t THREADS_COUNT = std::max<size_t>(std::thread::hardware_concurrency(), 1);
	// In order to create constexpr lambda, need to migrate project to C++17
	//static const size_t THREADS_COUNT = []() { return std::max<size_t>(std::thread::hardware_concurrency(), 1); }();
	size_t m_threadsCounter = THREADS_COUNT;

	std::mutex m_mutex;
	std::condition_variable m_conditional;

	FuncType m_evalFunc;
	FinishFuncType m_finishFunc;


private:
	const FinishFuncType ALWAYS_FALSE_FUNC = []() -> bool {
		return false;
	};
};

DETAIL_NAMESPACE_END

UBL_NAMESPACE_END