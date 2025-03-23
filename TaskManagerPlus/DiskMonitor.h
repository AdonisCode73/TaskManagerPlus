#pragma once
#include "MonitorUtils.h"

class DiskMonitor {
	public:
		void start();

		void stop();

	private:
		ULARGE_INTEGER availBytes;
		ULARGE_INTEGER totalBytes;
		ULARGE_INTEGER totalAvailBytes;

		PDH_HQUERY diskQuery;
		PDH_HCOUNTER readTotal;
		PDH_HCOUNTER writeTotal;
		PDH_HCOUNTER diskTime;

		PDH_FMT_COUNTERVALUE readCounterVal;
		PDH_FMT_COUNTERVALUE writeCounterVal;
		PDH_FMT_COUNTERVALUE diskTimeCounterVal;

		bool isRunning;
		std::thread diskThread;

		void update();

		void monitorLoop();

		void getUsage();
};
