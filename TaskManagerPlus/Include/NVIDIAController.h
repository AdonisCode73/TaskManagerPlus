#pragma once

#include "GpuMonitor.h"
#include "IGpuController.h"

#ifdef USE_NVIDIA
#include "nvml.h"

class NVIDIAController : public IGpuController {
public:
	void initNVML();

	void update() override;

private:
	nvmlReturn_t m_result;
};
#endif