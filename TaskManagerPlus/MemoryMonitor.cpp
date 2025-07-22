#include "MemoryMonitor.h"
#include "SystemStatus.h"


void MemoryMonitor::init() {
	m_memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	update();
}

double MemoryMonitor::calculateUtilisation() {
	return 100 - ((systemStatus.ramAvailMemory / systemStatus.ramTotalMemory) * 100); 
}

void MemoryMonitor::calculateMemory() {
	systemStatus.ramTotalMemory = m_memInfo.ullTotalPhys / (1024.0 * 1024.0 * 1024.0);
	systemStatus.ramAvailMemory = m_memInfo.ullAvailPhys / (1024.0 * 1024.0 * 1024.0);
}

void MemoryMonitor::update() {
	GlobalMemoryStatusEx(&m_memInfo);
	calculateMemory();
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


