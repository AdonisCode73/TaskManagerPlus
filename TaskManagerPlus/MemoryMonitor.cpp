#include "MemoryMonitor.h"

void MemoryMonitor::start() {
	update();
	m_isRunning = true;
	m_memoryThread = std::thread(&MemoryMonitor::monitorLoop, this);
}

void MemoryMonitor::update() {
	m_memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&m_memInfo);
	calculateTotal();
	calculateAvail();
}

void MemoryMonitor::stop() {
	m_isRunning = false;
	if (m_memoryThread.joinable()) {
		m_memoryThread.join();
	}
}

double MemoryMonitor::calculateUtilisation() {
	return 100 - ((systemStatus.ramAvailMemory / systemStatus.ramTotalMemory) * 100); 
}

void MemoryMonitor::calculateTotal() {
	systemStatus.ramTotalMemory = m_memInfo.ullTotalPhys / (1024.0 * 1024.0 * 1024.0);
}

void MemoryMonitor::calculateAvail() {
	systemStatus.ramAvailMemory = m_memInfo.ullAvailPhys / (1024.0 * 1024.0 * 1024.0);
}

void MemoryMonitor::monitorLoop() {
	while (m_isRunning) {
		update();
		/*std::lock_guard<std::mutex> lock(systemStatus.memoryMutex);*/
		MonitorUtils::checkQueueSize(systemStatus.memoryUsage);
		systemStatus.memoryUsage.push_front(calculateUtilisation());
		Sleep(1000);
	}
}

