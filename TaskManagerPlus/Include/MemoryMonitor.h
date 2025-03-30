#pragma once
#include "MonitorUtils.h"

class MemoryMonitor {

	public:
		void start();

		void stop();

	private:
		double totalPhysMem;
		double availPhysMem;
		double currentPhysMem;

		MEMORYSTATUSEX memInfo;

		std::thread memoryThread;
		bool isRunning = false;

		double calculateUtilisation();

		void calculateAvail();

		void calculateTotal();

		void monitorLoop();
		
		void update();
};
