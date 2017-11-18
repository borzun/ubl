#include "IRamMonitor.h"

#ifdef _WIN32
	#include "WindowsRamMonitor.h"
#endif

UBL_NAMESPACE_BEGIN

DETAIL_NAMESPACE_BEGIN

const IRamMonitor& IRamMonitor::getInstance()
{
#ifdef _WIN32
	static WindowsRamMonitor s_monitor;
	return s_monitor;
#else
	static_assert(false, "Invalid OS!");
#endif
}

DETAIL_NAMESPACE_END

UBL_NAMESPACE_END

