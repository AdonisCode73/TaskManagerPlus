#include "DiskMonitor.h"

void DiskMonitor::start() {
	update();

	PdhOpenQuery(nullptr, NULL, &diskQuery);
	PdhAddCounter(diskQuery, L"\\PhysicalDisk(_Total)\\Disk Read Bytes/sec", NULL, &readTotal);
	PdhAddCounter(diskQuery, L"\\PhysicalDisk(_Total)\\Disk Write Bytes/sec", NULL, &writeTotal);
	PdhAddCounter(diskQuery, L"\\PhysicalDisk(_Total)\\% Disk Time", 0, &diskTime);
	PdhCollectQueryData(diskQuery);

	isRunning = true;
	diskThread = std::thread(&DiskMonitor::monitorLoop, this);
}

void DiskMonitor::stop() {
	isRunning = false;
	if (diskThread.joinable()) {
		diskThread.join();
	}
}

void DiskMonitor::update() {
	GetDiskFreeSpaceEx(L"C:\\", &availBytes, &totalBytes, &totalAvailBytes);
	systemStatus.availDisk = totalAvailBytes.QuadPart / (1024.0 * 1024.0 * 1024.0);
	systemStatus.totalDisk = totalBytes.QuadPart / (1024.0 * 1024.0 * 1024.0);
}

void DiskMonitor::getUsage() {
	PdhCollectQueryData(diskQuery);
	PdhGetFormattedCounterValue(readTotal, PDH_FMT_DOUBLE, nullptr, &readCounterVal);
	PdhGetFormattedCounterValue(writeTotal, PDH_FMT_DOUBLE, nullptr, &writeCounterVal);
	PdhGetFormattedCounterValue(diskTime, PDH_FMT_DOUBLE, nullptr, &diskTimeCounterVal);
	
	systemStatus.readDisk = readCounterVal.doubleValue / 1024.0;
	systemStatus.writeDisk = writeCounterVal.doubleValue / 1024.0;
	/*std::lock_guard<std::mutex> lock(systemStatus.diskMutex);*/
	MonitorUtils::checkQueueSize(systemStatus.diskTime);
	systemStatus.diskTime.push_front(diskTimeCounterVal.doubleValue);
}

void DiskMonitor::monitorLoop() {
	while (isRunning) {
		getUsage();
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}
