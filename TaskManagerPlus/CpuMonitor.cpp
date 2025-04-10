#include "CpuMonitor.h"

void CpuMonitor::start() {
	PdhOpenQuery(nullptr, NULL, &cpuQuery);
	PdhAddEnglishCounter(cpuQuery, L"\\Processor(_Total)\\% Processor Time", NULL, &cpuTotal);
	PdhCollectQueryData(cpuQuery);

	isRunning = true;
	cpuThread = std::thread(&CpuMonitor::monitorLoop, this);
}

void CpuMonitor::stop() {
	isRunning = false;
	if (cpuThread.joinable()) {
		cpuThread.join();
	}
}

double CpuMonitor::getUsage() {
	PDH_FMT_COUNTERVALUE counterVal;

	PdhCollectQueryData(cpuQuery);
	PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, nullptr, &counterVal);
	return counterVal.doubleValue;
}

void CpuMonitor::monitorLoop() {
	while (isRunning) {
		/*std::lock_guard<std::mutex> lock(systemStatus.cpuMutex);*/
		MonitorUtils::checkQueueSize(systemStatus.cpuUsage);
		systemStatus.cpuUsage.push_front(getUsage());
		Sleep(1000);
	}
}