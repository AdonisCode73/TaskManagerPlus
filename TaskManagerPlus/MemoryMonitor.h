#pragma once

#include <windows.h>
#include <thread>

class MemoryMonitor {
	public:

		void start();

		void stop();

		void update();

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
};
