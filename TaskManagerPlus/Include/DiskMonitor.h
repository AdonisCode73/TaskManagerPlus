#pragma once
#include "MonitorUtils.h"

class DiskMonitor {
	public:
		void start();

		void stop();

	private:
		PDH_HQUERY m_diskQuery;
		PDH_HCOUNTER m_readTotal;
		PDH_HCOUNTER m_writeTotal;
		PDH_HCOUNTER m_diskTime;

		bool m_isRunning;
		std::thread m_diskThread;

		void update();

		void monitorLoop();

		void getUsage();
};
