#include "DiskMonitor.h"

void DiskMonitor::start() {
	update();

	PdhOpenQuery(nullptr, NULL, &m_diskQuery);
	PdhAddCounter(m_diskQuery, L"\\PhysicalDisk(_Total)\\Disk Read Bytes/sec", NULL, &m_readTotal);
	PdhAddCounter(m_diskQuery, L"\\PhysicalDisk(_Total)\\Disk Write Bytes/sec", NULL, &m_writeTotal);
	PdhAddCounter(m_diskQuery, L"\\PhysicalDisk(_Total)\\% Disk Time", 0, &m_diskTime);
	PdhCollectQueryData(m_diskQuery);

	m_isRunning = true;
	m_diskThread = std::thread(&DiskMonitor::monitorLoop, this);
}

void DiskMonitor::stop() {
	m_isRunning = false;
	if (m_diskThread.joinable()) {
		m_diskThread.join();
	}
}

void DiskMonitor::update() {
	ULARGE_INTEGER availBytes;
	ULARGE_INTEGER totalBytes;
	ULARGE_INTEGER totalAvailBytes;

	GetDiskFreeSpaceEx(L"C:\\", &availBytes, &totalBytes, &totalAvailBytes);
	systemStatus.availDisk = totalAvailBytes.QuadPart / (1024.0 * 1024.0 * 1024.0);
	systemStatus.totalDisk = totalBytes.QuadPart / (1024.0 * 1024.0 * 1024.0);
}

void DiskMonitor::getUsage() {
	PDH_FMT_COUNTERVALUE readCounterVal;
	PDH_FMT_COUNTERVALUE writeCounterVal;
	PDH_FMT_COUNTERVALUE diskTimeCounterVal;

	PdhCollectQueryData(m_diskQuery);
	PdhGetFormattedCounterValue(m_readTotal, PDH_FMT_DOUBLE, nullptr, &readCounterVal);
	PdhGetFormattedCounterValue(m_writeTotal, PDH_FMT_DOUBLE, nullptr, &writeCounterVal);
	PdhGetFormattedCounterValue(m_diskTime, PDH_FMT_DOUBLE, nullptr, &diskTimeCounterVal);
	
	systemStatus.readDisk = readCounterVal.doubleValue / 1024.0;
	systemStatus.writeDisk = writeCounterVal.doubleValue / 1024.0;
	/*std::lock_guard<std::mutex> lock(systemStatus.diskMutex);*/
	MonitorUtils::checkQueueSize(systemStatus.diskTime);
	systemStatus.diskTime.push_front(diskTimeCounterVal.doubleValue);
}

void DiskMonitor::monitorLoop() {
	while (m_isRunning) {
		getUsage();
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}
