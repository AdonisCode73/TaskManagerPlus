#pragma once
#include <atomic>
#include <thread>
#include <tchar.h>
#include <Pdh.h>

class CpuMonitor {
public:
	void start();

	void stop();

	double getUsage();
private:
	std::atomic<double> currentVal{ 0.0 };
	std::thread monitorThread;
	bool isRunning = false;

	PDH_HQUERY cpuQuery;
	PDH_HCOUNTER cpuTotal;

	void monitorLoop();
};
