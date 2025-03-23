#include "DiskMonitor.h"

void DiskMonitor::start() {
	update();

	PdhOpenQuery(NULL, NULL, &diskQuery);
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
	PDH_FMT_COUNTERVALUE readCounterVal;
	PDH_FMT_COUNTERVALUE writeCounterVal;
	PDH_FMT_COUNTERVALUE diskTimeCounterVal;

	PdhCollectQueryData(diskQuery);
	PdhGetFormattedCounterValue(readTotal, PDH_FMT_DOUBLE, NULL, &readCounterVal);
	PdhGetFormattedCounterValue(writeTotal, PDH_FMT_DOUBLE, NULL, &writeCounterVal);
	PdhGetFormattedCounterValue(diskTime, PDH_FMT_DOUBLE, NULL, &diskTimeCounterVal);
	
	systemStatus.readDisk = readCounterVal.doubleValue / 1024.0;
	systemStatus.writeDisk = writeCounterVal.doubleValue / 1024.0;
	systemStatus.diskTime = diskTimeCounterVal.doubleValue;
}

void DiskMonitor::monitorLoop() {
	while (isRunning) {
		getUsage();
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}
