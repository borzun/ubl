#pragma once

#include "ubl.h"
#include <string>
#include <chrono>

UBL_NAMESPACE_BEGIN

template <class TimerType>
class ElapsedTimer final
{
public:
	ElapsedTimer();

	std::chrono::milliseconds elapsedMsecs() const noexcept;
	std::chrono::nanoseconds elapsedNanos() const noexcept;

private:

	ElapsedTimer(const ElapsedTimer&) = delete;
	ElapsedTimer& operator=(const ElapsedTimer&) = delete;

private:
	typename TimerType::time_point m_startTimePoint;
};

// Convenient definition
using default_timer_t = std::chrono::system_clock;

using PreciseElapsedTimer = ElapsedTimer<std::chrono::high_resolution_clock>;
using DefaultElapsedTimer = ElapsedTimer<default_timer_t>;

UBL_NAMESPACE_END
