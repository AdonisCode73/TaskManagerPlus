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
	ADLXHelper adlxHelper;

	IADLXGPUMetricsPtr gpuMetrics;
	IADLXGPUMetricsSupportPtr gpuMetricsSupport;

	adlx_double gpuUsage;
	adlx_int usedVram;
	adlx_int gpuClockSpeed;
	adlx_double gpuTemp;
	adlx_int fanSpeed;

	IADLXPerformanceMonitoringServicesPtr perfMonitoringService;
	IADLXGPUPtr oneGPU;
};
#endif