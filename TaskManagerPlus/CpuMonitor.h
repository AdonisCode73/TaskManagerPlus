#pragma once
#include <tchar.h>
#include "MonitorUtils.h"

class CpuMonitor {

	public:
		void start();

		void stop();

	private:
		std::thread cpuThread;
		bool isRunning;

		PDH_HQUERY cpuQuery;
		PDH_HCOUNTER cpuTotal;

		void monitorLoop();
		
		double getUsage();
};
