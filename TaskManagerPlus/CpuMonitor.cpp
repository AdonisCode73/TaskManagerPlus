#include "CpuMonitor.h"

void CpuMonitor::start() {
	PdhOpenQuery(nullptr, NULL, &m_cpuQuery);
	PdhAddEnglishCounter(m_cpuQuery, L"\\Processor(_Total)\\% Processor Time", NULL, &m_cpuTotal);
	PdhCollectQueryData(m_cpuQuery);

	m_isRunning = true;
	m_cpuThread = std::thread(&CpuMonitor::monitorLoop, this);
}

void CpuMonitor::stop() {
	m_isRunning = false;
	if (m_cpuThread.joinable()) {
		m_cpuThread.join();
	}
}

double CpuMonitor::getUsage() {
	PDH_FMT_COUNTERVALUE counterVal;

	PdhCollectQueryData(m_cpuQuery);
	PdhGetFormattedCounterValue(m_cpuTotal, PDH_FMT_DOUBLE, nullptr, &counterVal);
	return counterVal.doubleValue;
}

void CpuMonitor::monitorLoop() {
	while (m_isRunning) {
		/*std::lock_guard<std::mutex> lock(systemStatus.cpuMutex);*/
		MonitorUtils::checkQueueSize(systemStatus.cpuUsage);
		systemStatus.cpuUsage.push_front(getUsage());
		Sleep(1000);
	}
}