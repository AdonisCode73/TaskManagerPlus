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
#include "NetworkMonitor.h"
#include "GuiController.h"
#include <queue>

#define MAX_QUEUE_SIZE 73
struct SystemStatus {
	std::queue <double> cpuUsage;
	/*std::mutex cpuMutex;*/

	std::queue <double> memoryUsage;
	/*std::mutex memoryMutex;*/
	std::atomic <double> ramAvailMemory{ 0.0 };
	std::atomic <double> ramTotalMemory{ 0.0 };

	std::atomic <double> vramAvailMemory{ 0.0 };
	std::atomic <double> vramTotalMemory{ 0.0 };
	std::queue <double> gpuUsage;
	/*std::mutex gpuMutex;*/
	std::atomic <double> memControllerUsage{ 0.0 };
	
	std::atomic <double> totalDisk{ 0.0 };
	std::atomic <double> availDisk{ 0.0 };
	std::atomic <double> readDisk{ 0.0 };
	std::atomic <double> writeDisk{ 0.0 };
	std::queue <double> diskTime;
	/*std::mutex diskMutex;*/
	
	std::atomic <double> sendNetwork{ 0.0 };
	std::atomic <double> receiveNetwork{ 0.0 };
};
extern	SystemStatus systemStatus;

class MonitorUtils {
	public:
		void start();

		void stop();

		bool isRunningCheck();

		static void checkQueueSize(std::queue <double>);

	private:
		COORD initialCursorPos;

		std::thread monitorThread;

		void saveCursorPosition();

		void restoreCursorPosition();

		void monitorUtil();

		bool isRunning = false;

		bool jumpCursor = true;
};