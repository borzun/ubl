#include "MacosRamMonitor.h"

#include <sys/time.h>
#include <sys/resource.h>

UBL_NAMESPACE_BEGIN

DETAIL_NAMESPACE_BEGIN

size_t MacosRamMonitor::getUsedMemoryByCurrentProcess() const
{
    rusage stats;
	getrusage(RUSAGE_SELF, &stats);
	return stats.ru_maxrss;
}

DETAIL_NAMESPACE_END

UBL_NAMESPACE_END
