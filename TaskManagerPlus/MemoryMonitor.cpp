#include "MemoryMonitor.h"

void MemoryMonitor::start() {
	update();
	isRunning = true;
	memoryThread = std::thread(&MemoryMonitor::monitorLoop, this);
}

void MemoryMonitor::update() {
	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memInfo);
	calculateTotal();
	calculateAvail();
}

void MemoryMonitor::stop() {
	isRunning = false;
	if (memoryThread.joinable()) {
		memoryThread.join();
	}
}

double MemoryMonitor::calculateUtilisation() {
	return 100 - ((systemStatus.ramAvailMemory / systemStatus.ramTotalMemory) * 100); 
}

void MemoryMonitor::calculateTotal() {
	systemStatus.ramTotalMemory = memInfo.ullTotalPhys / (1024.0 * 1024.0 * 1024.0);
}

void MemoryMonitor::calculateAvail() {
	systemStatus.ramAvailMemory = memInfo.ullAvailPhys / (1024.0 * 1024.0 * 1024.0);
}

void MemoryMonitor::monitorLoop() {
	while (isRunning) {
		update();
		systemStatus.memoryUsage = calculateUtilisation();
	}
}

