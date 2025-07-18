#include "ADLXController.h"

#ifdef USE_AMD
void ADLXController::initADLX() {
	ADLX_RESULT res = ADLX_FAIL;

	res = m_adlxHelper.Initialize();

	if (ADLX_SUCCEEDED(res)) {
		res = m_adlxHelper.GetSystemServices()->GetPerformanceMonitoringServices(&m_perfMonitoringService);
		if (ADLX_SUCCEEDED(res))
		{
			IADLXGPUListPtr gpus;
			res = m_adlxHelper.GetSystemServices()->GetGPUs(&gpus);
			if (ADLX_SUCCEEDED(res))
			{
				res = gpus->At(gpus->Begin(), &m_oneGPU);
				if (ADLX_SUCCEEDED(res))
				{
				}
				else {
					std::cout << "Get particular GPU failed" << std::endl;
				}
			}
			else {
				std::cout << "Get GPU list failed" << std::endl;
			}
		}
		else {
			std::cout << "Get performance monitoring services failed" << std::endl;
		}
	}
}

void ADLXController::update() {

	IADLXGPUMetricsPtr gpuMetrics;
	IADLXGPUMetricsSupportPtr gpuMetricsSupport;
	m_perfMonitoringService->GetCurrentGPUMetrics(m_oneGPU, &gpuMetrics);

	adlx_int usedVram;
	gpuMetrics->GPUVRAM(&usedVram);
	systemStatus.vramAvailMemory = (double)(systemStatus.vramTotalMemory - (usedVram / 1024.0));

	adlx_int gpuClockSpeed;
	gpuMetrics->GPUClockSpeed(&gpuClockSpeed);
	systemStatus.gpuClockSpeed = (double)gpuClockSpeed;

	adlx_double gpuTemp;
	gpuMetrics->GPUTemperature(&gpuTemp);
	systemStatus.gpuTemperature = (double)gpuTemp;

	adlx_int fanSpeed;
	gpuMetrics->GPUFanSpeed(&fanSpeed);
	systemStatus.gpuFanSpeed = (double)fanSpeed;

	adlx_double gpuUsage;
	gpuMetrics->GPUUsage(&gpuUsage);
	{
		std::lock_guard<std::mutex> lock(systemStatus.gpuMutex);
		MonitorUtils::checkQueueSize(systemStatus.gpuUsage);
		systemStatus.gpuUsage.push_front((double)gpuUsage);
	}
}
#endif