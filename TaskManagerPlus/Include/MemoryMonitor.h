#pragma once
#include <thread>
#include <windows.h>
#include "MonitorBase.h"

class MemoryMonitor : public MonitorBase {

	public:

	private:
		MEMORYSTATUSEX m_memInfo{};

		double calculateUtilisation();

		void calculateAvail();

		void calculateTotal();

		void monitorLoop() override;

		void init() override;
		
		void update();
};
