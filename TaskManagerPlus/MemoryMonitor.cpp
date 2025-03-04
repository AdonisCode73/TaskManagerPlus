#include <iomanip>
#include <iostream>
#include <cmath>
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
	return 100 - ((availPhysMem / totalPhysMem) * 100);
}

void MemoryMonitor::calculateTotal() {
	totalPhysMem = memInfo.ullTotalPhys / (1024.0 * 1024.0 * 1024.0);
}

void MemoryMonitor::calculateAvail() {
	availPhysMem = memInfo.ullAvailPhys / (1024.0 * 1024.0 * 1024.0);
}

void MemoryMonitor::monitorLoop() {
	std::cout << "You have " << std::fixed << std::setprecision(1) << totalPhysMem << "GB total memory available in your system.\n";
	std::cout << "You have " << std::fixed << std::setprecision(1) << availPhysMem << "GB available memory available in your system.\n";
	while (isRunning) {
		update();
		currentPhysMem = calculateUtilisation();
		std::cout << "\r" << std::string(30, ' ') << "\r";
		std::cout << "Current memory utilisation: " << std::fixed << currentPhysMem << std::setprecision(1) << "%" << std::flush;
		Sleep(1000);
	}
}

