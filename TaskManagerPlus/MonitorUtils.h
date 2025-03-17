#pragma once
#include <mutex>
#include <thread>
#include <windows.h>
#include <iostream>
#include <iomanip>

struct SystemStatus {
	std::atomic <double> cpuUsage{ 0.0 };

	std::atomic <double> memoryUsage{ 0.0 };
	std::atomic <double> ramAvailMemory{ 0.0 };
	std::atomic <double> ramTotalMemory{ 0.0 };

	std::atomic <double> vramAvailMemory{ 0.0 };
	std::atomic <double> vramTotalMemory{ 0.0 };
	std::atomic <double> gpuUsage{ 0.0 };
	std::atomic <double> memControllerUsage{ 0.0 };
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