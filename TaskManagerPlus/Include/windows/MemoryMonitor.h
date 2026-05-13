#pragma once
#include <thread>
#include <windows.h>
#include "MonitorBase.h"

class MemoryMonitor : public MonitorBase {

	public:

	private:
		MEMORYSTATUSEX m_memInfo{};;

		void monitorLoop() override;

		void init() override;

		void update();

		void calculateMemory();

		double calculateUtilisation();
};
