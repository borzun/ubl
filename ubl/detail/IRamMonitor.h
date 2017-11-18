#pragma once

#include "ubl.h"

UBL_NAMESPACE_BEGIN

DETAIL_NAMESPACE_BEGIN

class IRamMonitor
{
public:
	
	virtual size_t getUsedMemoryByCurrentProcess() const = 0;

	static const IRamMonitor& getInstance();

protected:
	
	IRamMonitor() = default;
	virtual ~IRamMonitor() = default;

private:
	IRamMonitor(const IRamMonitor&) = delete;
	IRamMonitor& operator=(const IRamMonitor&) = delete;
};

DETAIL_NAMESPACE_END

UBL_NAMESPACE_END
