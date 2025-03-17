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
		systemStatus.cpuUsage = getUsage();
		Sleep(1000);
	}
}