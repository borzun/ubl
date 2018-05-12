#pragma once

#pragma once

#include <ubl.h>


UBL_NAMESPACE_BEGIN

DETAIL_NAMESPACE_BEGIN

template <class IterType, class ValueType>
class ParallelSearchOpenMP
{
private:

	bool m_returnValue = false;

public:

	ParallelSearchOpenMP(IterType begin, IterType end, const ValueType& value)
	{
		#pragma omp parallel for
		for (auto iter = begin; iter != end; ++iter) {
			if (*iter == value) {
				m_returnValue = true;
				break;
			}
		}
	}

	bool isFound() const noexcept
	{
		return m_returnValue;
	}

	bool operator()() const {
		return isFound();
	}

private:
	
};

DETAIL_NAMESPACE_END

UBL_NAMESPACE_END