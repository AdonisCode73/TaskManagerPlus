#pragma once
#include <tchar.h>
#include "MonitorUtils.h"

class CpuMonitor {

	public:
		void start();
		void stop();

	private:
		std::thread m_cpuThread;
		bool m_isRunning = true;

		PDH_HQUERY m_cpuQuery = nullptr;
		PDH_HCOUNTER m_cpuTotal = nullptr;

		void monitorLoop();
		double getUsage();
};
