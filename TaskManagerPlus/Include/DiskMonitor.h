#pragma once
#include "MonitorUtils.h"

class DiskMonitor {
	public:
		void start();

		void stop();

	private:
		PDH_HQUERY m_diskQuery = nullptr;
		PDH_HCOUNTER m_readTotal = nullptr;
		PDH_HCOUNTER m_writeTotal = nullptr;
		PDH_HCOUNTER m_diskTime = nullptr;

		bool m_isRunning = true;
		std::thread m_diskThread;

		void update();

		void monitorLoop();

		void getUsage();
};
