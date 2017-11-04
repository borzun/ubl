#include "ElapsedTimer.h"

UBL_NAMESPACE_BEGIN

template<class TimerType>
inline ElapsedTimer<TimerType>::ElapsedTimer()
	: m_startTimePoint(TimerType::now())
{
}

template<class TimerType>
inline std::chrono::milliseconds ElapsedTimer<TimerType>::elapsedMsecs() const noexcept
{
	auto elapsedTime = TimerType::now() - m_startTimePoint;
	return std::chrono::duration_cast<std::chrono::milliseconds>(elapsedTime);
}

template<class TimerType>
inline std::chrono::nanoseconds ElapsedTimer<TimerType>::elapsedNanos() const noexcept
{
	auto elapsedTime = TimerType::now() - m_startTimePoint;
	return std::chrono::duration_cast<std::chrono::nanoseconds>(elapsedTime);
}

// Explicit instantionation:
template class ElapsedTimer<default_timer_t>;
template class ElapsedTimer<std::chrono::high_resolution_clock>;
template class ElapsedTimer<std::chrono::steady_clock>;

UBL_NAMESPACE_END