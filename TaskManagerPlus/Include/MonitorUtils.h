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
#include <deque>

#define MAX_QUEUE_SIZE 73
struct SystemStatus {
	std::deque <double> cpuUsage{ 0.0 };
	/*std::mutex cpuMutex;*/

	std::deque <double> memoryUsage{ 0.0 };
	/*std::mutex memoryMutex;*/
	std::atomic <double> ramAvailMemory{ 0.0 };
	std::atomic <double> ramTotalMemory{ 0.0 };

	std::atomic <double> vramAvailMemory{ 0.0 };
	std::atomic <double> gpuClockSpeed { 0.0 };
	std::atomic <double> gpuFanSpeed { 0.0 };
	std::atomic <double> vramTotalMemory{ 0.0 };
	std::deque <double> gpuUsage{ 0.0 };
	/*std::mutex gpuMutex;*/
	std::atomic <double> memControllerUsage{ 0.0 };
	std::atomic <double> gpuTemperature { 0.0 };
	
	std::atomic <double> totalDisk{ 0.0 };
	std::atomic <double> availDisk{ 0.0 };
	std::atomic <double> readDisk{ 0.0 };
	std::atomic <double> writeDisk{ 0.0 };
	std::deque <double> diskTime{ 0.0 };
	/*std::mutex diskMutex;*/
	
	std::atomic <double> sendNetwork{ 0.0 };
	std::atomic <double> receiveNetwork{ 0.0 };
	std::deque <double> networkUsage{ 0.0 };
};
extern	SystemStatus systemStatus;

class MonitorUtils {
	public:
		void start();

		void stop();

		bool isRunningCheck();

		static void checkQueueSize(std::deque <double>);

	private:
		bool isRunning = false;
};