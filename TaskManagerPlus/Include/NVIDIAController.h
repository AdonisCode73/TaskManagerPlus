#pragma once

#include "GpuMonitor.h"

#ifdef USE_NVIDIA
#include "nvml.h"

class NVIDIAController {
public:
	void initNVML();

	void update();

private:
	nvmlReturn_t m_result;
};
#endif