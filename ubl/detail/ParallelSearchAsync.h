#pragma once

#include <ubl.h>

#include <future>
#include <thread>
#include <condition_variable>

UBL_NAMESPACE_BEGIN

DETAIL_NAMESPACE_BEGIN

template <class IterType, class ValueType>
class ParallelSearchAsync
{
private:

	const ValueType& m_value;

	size_t m_threadsCounter;

	std::mutex m_mutex;
	std::condition_variable m_conditional;

	bool m_returnValue = false;

public:

	ParallelSearchAsync(IterType begin, IterType end, const ValueType& value)
		: m_value(value)
	{
		const auto SIZE = end - begin;
		int threads_count = std::thread::hardware_concurrency();
		m_threadsCounter = threads_count;

		const auto bucket_size = SIZE / threads_count;
		auto curr_iter = begin;

		std::vector<std::thread> threads;

		for (int i = 0; i < threads_count; ++i) {
			const auto next_size = (i < threads_count - 1) ? bucket_size
				: SIZE - (bucket_size * (threads_count - 1));

			auto next_iter = std::next(curr_iter, next_size);
			auto handle = std::async(std::launch::async,
				&ParallelSearchAsync<IterType, ValueType>::performParallelSearch,
				this, curr_iter, next_iter);

			curr_iter = next_iter;
			curr_iter = next_iter;
		}

		std::unique_lock<std::mutex> guard(m_mutex);
		m_conditional.wait(guard, [this]() { return m_threadsCounter == 0 || m_returnValue; });
	}

	bool isFound() const noexcept
	{
		return m_returnValue;
	}

	bool operator()() const {
		return isFound();
	}

private:
	void performParallelSearch(IterType begin, IterType end)
	{
		auto foundIter = std::find(begin, end, m_value);

		{
			std::unique_lock<std::mutex> guard(m_mutex);
			
			if (foundIter != end) {
				m_returnValue = true;
			}

			--m_threadsCounter;
			if (m_threadsCounter == 0 || m_returnValue) {
				m_conditional.notify_one();
			}
		}
	}
};

DETAIL_NAMESPACE_END

UBL_NAMESPACE_END