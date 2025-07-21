#include "MemoryMonitor.h"
#include "SystemStatus.h"

void MemoryMonitor::update() {
	m_memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&m_memInfo);
	calculateTotal();
	calculateAvail();
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
		{
			std::lock_guard<std::mutex> lock(systemStatus.memoryMutex);
			systemStatus.checkQueueSize(systemStatus.memoryUsage);
			systemStatus.memoryUsage.push_front(calculateUtilisation());
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

void MemoryMonitor::init() {
	update();
}

