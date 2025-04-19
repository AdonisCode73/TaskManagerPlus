#pragma once
#include <tchar.h>
#include "MonitorUtils.h"

class CpuMonitor {

	public:
		void start();
		void stop();

	private:
		std::thread m_cpuThread;
		bool m_isRunning;

		PDH_HQUERY m_cpuQuery;
		PDH_HCOUNTER m_cpuTotal;

		void monitorLoop();
		double getUsage();
};
