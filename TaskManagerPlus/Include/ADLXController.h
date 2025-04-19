#pragma once

#include "GpuMonitor.h"

#ifdef USE_AMD
#include "ADLXHelper.h"
#include "IPerformanceMonitoring.h"
#include "IPerformanceMonitoring2.h"

class ADLXController {
public:
	void initADLX();

	void update();

private:

	ADLXHelper m_adlxHelper;

	IADLXPerformanceMonitoringServicesPtr m_perfMonitoringService;
	IADLXGPUPtr m_oneGPU;
};
#endif