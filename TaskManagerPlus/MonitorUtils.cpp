#include "MonitorUtils.h"

CpuMonitor cpuMonitor;
MemoryMonitor memoryMonitor;
GpuMonitor gpuMonitor;
DiskMonitor diskMonitor;
NetworkMonitor networkMonitor;
GuiController guiController;

SystemStatus systemStatus;


void MonitorUtils::start() {
	cpuMonitor.start();
	memoryMonitor.start();
	gpuMonitor.start();
	diskMonitor.start();
	networkMonitor.start();
	guiController.start();
}

bool MonitorUtils::isRunningCheck() {
	if (std::cin.get() == '\n') {
		return false;
	}
	return true;
}

void MonitorUtils::stop() {
	cpuMonitor.stop();
	memoryMonitor.stop();
	gpuMonitor.stop();
	diskMonitor.stop();
	networkMonitor.stop();
}

void MonitorUtils::checkQueueSize(std::deque <double> statQueue) {
	if (statQueue.size() == MAX_QUEUE_SIZE) {
		statQueue.pop_back();
	}
}

