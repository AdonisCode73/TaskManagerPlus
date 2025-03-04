#include <tchar.h>
#include <iostream>
#include <Pdh.h>
#include <atomic>
#include <iomanip>
#include "CpuMonitor.h"

void CpuMonitor::start() {
	PdhOpenQuery(NULL, NULL, &cpuQuery);
	PdhAddEnglishCounter(cpuQuery, L"\\Processor(_Total)\\% Processor Time", NULL, &cpuTotal);
	PdhCollectQueryData(cpuQuery);

	isRunning = true;
	monitorThread = std::thread(&CpuMonitor::monitorLoop, this);
}

void CpuMonitor::stop() {
	isRunning = false;
	if (monitorThread.joinable()) {
		monitorThread.join();
	}
}

double CpuMonitor::getUsage() {
	PDH_FMT_COUNTERVALUE counterVal;

	PdhCollectQueryData(cpuQuery);
	PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);
	return counterVal.doubleValue;
}

void CpuMonitor::monitorLoop() {
	while (isRunning) {
		currentVal = getUsage();
		std::cout << "\r" << std::string(30, ' ') << "\r";
		std::cout << "CPU Usage: " << std::fixed << std::setprecision(1) << currentVal << " %" << std::flush;
		Sleep(1000);
	}
}