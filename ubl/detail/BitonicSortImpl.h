#pragma once

#include <vector>
#include <cassert>

#include "ubl.h"
#include "ThreadPool.h"

UBL_NAMESPACE_BEGIN

DETAIL_NAMESPACE_BEGIN

template <class Iterator>
class BitonicSortImpl
{
public:

	BitonicSortImpl(Iterator begin, Iterator end) noexcept
		: m_begin(begin)
		, m_end(end)
	{
		m_size = std::distance(begin, end);
	}

	__forceinline void bitonicSwap(Iterator iterBegin, size_t step, bool direction) noexcept
	{
		auto iterEnd = iterBegin;
		std::advance(iterEnd, step);

		bool isCorrect = *iterBegin > *iterEnd;

		// If moving in previous direction, perform reversed compare
		if (!direction) {
			isCorrect = !isCorrect;
		}

		if (isCorrect) {
			std::swap(*iterBegin, *iterEnd);
		}
	}

	void performBitonicStep(Iterator iterBegin, size_t size, bool direction) noexcept
	{
		if (size == 1) {
			return;
		}

		const auto halfStep = size / 2;
		auto iterHalf = iterBegin;
		std::advance(iterHalf, halfStep);

		performBitonicStep(iterBegin, halfStep, true);
		performBitonicStep(iterHalf, halfStep, false);

		bitonicMerge(iterBegin, size, direction);
	}

	void bitonicMergeStep(Iterator iterBegin, size_t step, bool direction) noexcept
	{
		bitonicMergeStepImpl(iterBegin, step, direction, 0, 1);
	}

	void bitonicMergeStepParallel(Iterator iterBegin, size_t step, bool direction) noexcept
	{
		ThreadPool& sharedThreadPool = ThreadPool::getInstance();
		size_t threadsCount = sharedThreadPool.getThreadsCount();

		bool parallel_execution = (step / threadsCount) > 1024;
		if (parallel_execution) {
			for (int i = 0; i < threadsCount; ++i) {
				sharedThreadPool.run([=]() {
					this->bitonicMergeStepImpl(iterBegin, step, direction, i, threadsCount);
				});
			}

			// Wait till all other threads are finished
			while (!sharedThreadPool.isIdle());
		}
		else {
			bitonicMergeStepImpl(iterBegin, step, direction, 0, 1);
		}
	}

	void bitonicMergeStepImpl(Iterator iterBegin, size_t step, bool direction, size_t offset, size_t advance_step) noexcept
	{
		auto iterStart = iterBegin;
		std::advance(iterStart,  offset);

		auto iterEnd = iterBegin;
		std::advance(iterEnd, step - advance_step + (offset + 1));

		auto iter = iterStart;
		while (iter < iterEnd) {
			bitonicSwap(iter, step, direction);
			std::advance(iter, advance_step);
		}
	}

	void bitonicMerge(Iterator iterBegin, size_t size, bool direction) noexcept
	{
		if (size == 1) {
			return;
		}

		const auto step = size / 2;
		
		bitonicMergeStep(iterBegin, step, direction);

		auto iterHalf = iterBegin;
		std::advance(iterHalf, step);
		
		bitonicMerge(iterBegin, step, direction);
		bitonicMerge(iterHalf,	step, direction);
	}

	void sort() noexcept
	{
		performBitonicStep(m_begin, m_size, true);
	}

	
private:
	[[deprecated]] void performBitonicStep_old(Iterator iterBegin, size_t step, bool direction)
	{
		auto iterEnd = iterBegin;
		std::advance(iterEnd, step);

		bool is_correct = *iterEnd > *iterBegin;
		//is_correct ^= !direction;
		if (!direction) {
			is_correct = !is_correct;
		}

		if (!is_correct) {
			std::swap(*iterBegin, *iterEnd);
		}
	}

private:
	Iterator m_begin,
			 m_end;

	size_t m_size;
};


DETAIL_NAMESPACE_END


template<class Iterator>
inline void bitonicSort(Iterator begin, Iterator end)
{
	detail::BitonicSortImpl<decltype(begin)> sorter(begin, end);
	sorter.sort();
}


inline void testAPI()
{
	std::vector<int> vec = { 1, 2, 3 };
	bitonicSort(vec);
}


UBL_NAMESPACE_END
