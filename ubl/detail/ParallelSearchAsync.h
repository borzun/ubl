#pragma once

#include <ubl.h>

#include <atomic>
#include "patterns/MasterSlavePattern.h"

UBL_NAMESPACE_BEGIN

DETAIL_NAMESPACE_BEGIN

template <class IterType, class ValueType>
class ParallelSearchAsync
{
private:

	const ValueType& m_value;
	std::atomic<bool> m_returnValue;

public:

	ParallelSearchAsync(IterType begin, IterType end, const ValueType& value)
		: m_value(value)
		, m_returnValue(false)
	{
		using namespace std::placeholders;

		auto evalFunc = std::bind(&ParallelSearchAsync<IterType, ValueType>::performParallelSearch, this, _1, _2);
		auto isFoundFunc = std::bind(&ParallelSearchAsync<IterType, ValueType>::isFound, this);
		MasterSlavePattern<IterType> slaver(begin, end, evalFunc, isFoundFunc);
	}

	inline bool isFound() const noexcept
	{
		return m_returnValue.load();
	}

	inline bool operator()() const noexcept {
		return isFound();
	}

	ParallelSearchAsync(const ParallelSearchAsync& other) = delete;
	ParallelSearchAsync& operator=(const ParallelSearchAsync& other) = delete;

private:
	void performParallelSearch(IterType begin, IterType end)
	{
		auto foundIter = std::find(begin, end, m_value);
		if (foundIter != end) {
			m_returnValue.store(true);
		}
	}
};

DETAIL_NAMESPACE_END

UBL_NAMESPACE_END
