#include "WindowsRamMonitor.h"

#include "windows.h"
#include "psapi.h"

UBL_NAMESPACE_BEGIN

DETAIL_NAMESPACE_BEGIN

size_t WindowsRamMonitor::getUsedMemoryByCurrentProcess() const
{
	PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
	return pmc.PagefileUsage;
}

DETAIL_NAMESPACE_END

UBL_NAMESPACE_END
