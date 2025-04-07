#include "ADLXController.h"


void ADLXController::initADLX() {
	ADLX_RESULT  res = ADLX_FAIL;

	res = adlxHelper.Initialize();

	if (ADLX_SUCCEEDED(res)) {
		res = adlxHelper.GetSystemServices()->GetPerformanceMonitoringServices(&perfMonitoringService);
		if (ADLX_SUCCEEDED(res))
		{
			IADLXGPUListPtr gpus;
			res = adlxHelper.GetSystemServices()->GetGPUs(&gpus);
			if (ADLX_SUCCEEDED(res))
			{
				res = gpus->At(gpus->Begin(), &oneGPU);
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
	perfMonitoringService->GetCurrentGPUMetrics(oneGPU, &gpuMetrics);

	gpuMetrics->GPUVRAM(&usedVram);
	systemStatus.vramAvailMemory = (double)(systemStatus.vramTotalMemory - (usedVram / 1024.0));

	gpuMetrics->GPUClockSpeed(&gpuClockSpeed);
	systemStatus.gpuClockSpeed = (double)gpuClockSpeed;

	gpuMetrics->GPUTemperature(&gpuTemp);
	systemStatus.gpuTemperature = (double)gpuTemp;

	gpuMetrics->GPUFanSpeed(&fanSpeed);
	systemStatus.gpuFanSpeed = (double)fanSpeed;

	gpuMetrics->GPUUsage(&gpuUsage);
	MonitorUtils::checkQueueSize(systemStatus.gpuUsage);
	systemStatus.gpuUsage.push_front((double)gpuUsage);
}