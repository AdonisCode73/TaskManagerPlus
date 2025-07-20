#pragma once

#include "GpuMonitor.h"
#include "IGpuController.h"

class NVIDIAController : public IGpuController {
public:
	void init() override;

	void update() override;
};