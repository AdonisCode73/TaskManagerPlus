#pragma once

#include "GpuMonitor.h"
#include "IGpuController.h"

#ifdef USE_AMD
#include "ADLXHelper.h"
#include "IPerformanceMonitoring.h"
#include "IPerformanceMonitoring2.h"
#endif

class ADLXController : public IGpuController {
public:
	void init() override;

	void update() override;

private:

#ifdef USE_AMD
	ADLXHelper m_adlxHelper;

	IADLXPerformanceMonitoringServicesPtr m_perfMonitoringService;
	IADLXGPUPtr m_oneGPU;
#endif
};