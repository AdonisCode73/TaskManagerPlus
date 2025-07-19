#pragma once
#include "MonitorUtils.h"

class MemoryMonitor {

	public:
		void start();

		void stop();

	private:
		MEMORYSTATUSEX m_memInfo{};

		std::thread m_memoryThread;
		bool m_isRunning = false;

		double calculateUtilisation();

		void calculateAvail();

		void calculateTotal();

		void monitorLoop();
		
		void update();
};
