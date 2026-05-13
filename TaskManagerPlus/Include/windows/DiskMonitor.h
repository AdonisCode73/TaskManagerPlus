#pragma once
#include <thread>
#include <Pdh.h>
#include "MonitorBase.h"

class DiskMonitor : public MonitorBase {
	public:
	private:
		PDH_HQUERY m_diskQuery = nullptr;
		PDH_HCOUNTER m_readTotal = nullptr;
		PDH_HCOUNTER m_writeTotal = nullptr;
		PDH_HCOUNTER m_diskTime = nullptr;

		void update();

		void monitorLoop() override;

		void init() override;

		void getUsage();
};
