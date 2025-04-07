#pragma once

#include "GpuMonitor.h"

#ifdef USE_NVIDIA
#include "nvml.h"

class NVIDIAController {
public:
	void initNVML();

	void update();

private:
	nvmlDevice_t device;
	nvmlReturn_t result;
	nvmlMemory_t memInfo;
	nvmlUtilization_t utilization;
};
#endif