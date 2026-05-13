#pragma once
#include <tchar.h>
#include <thread>
#include <Pdh.h>
#include "MonitorBase.h"

class CpuMonitor : public MonitorBase {
	public:

	private:
		PDH_HQUERY m_cpuQuery = nullptr;
		PDH_HCOUNTER m_cpuTotal = nullptr;

		void monitorLoop() override;

		void init() override;

		double getUsage();
};
