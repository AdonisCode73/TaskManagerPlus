#pragma once
#include <mutex>
#include <thread>
#include <iostream>
#include <iomanip>
#include <windows.h>
#include <Pdh.h>

#include "CpuMonitor.h"
#include "MemoryMonitor.h"
#include "GpuMonitor.h"
#include "DiskMonitor.h"

struct SystemStatus {
	std::atomic <double> cpuUsage{ 0.0 };

	std::atomic <double> memoryUsage{ 0.0 };
	std::atomic <double> ramAvailMemory{ 0.0 };
	std::atomic <double> ramTotalMemory{ 0.0 };

	std::atomic <double> vramAvailMemory{ 0.0 };
	std::atomic <double> vramTotalMemory{ 0.0 };
	std::atomic <double> gpuUsage{ 0.0 };
	std::atomic <double> memControllerUsage{ 0.0 };
	
	std::atomic <double> totalDisk{ 0.0 };
	std::atomic <double> availDisk{ 0.0 };
	std::atomic <double> readDisk{ 0.0 };
	std::atomic <double> writeDisk{ 0.0 };
	std::atomic <double> diskTime{ 0.0 };
};
extern	SystemStatus systemStatus;

class MonitorUtils {
	public:
		void start();

		void stop();

		bool isRunningCheck();

	private:
		COORD initialCursorPos;

		std::thread monitorThread;

		void saveCursorPosition();

		void restoreCursorPosition();

		void monitorUtil();

		bool isRunning = false;

		bool jumpCursor = true;
};