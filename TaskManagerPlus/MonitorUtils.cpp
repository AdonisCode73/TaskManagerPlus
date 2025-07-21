#include "MonitorUtils.h"
#include "SystemStatus.h"
#include <iostream>

SystemStatus systemStatus;

void MonitorUtils::init() {
	launchMonitors();

	while (systemStatus.shutdownFlag == false) {

	}

	std::cout << "Beginning Shutdown Operations.";

	shutdownMonitors();
}

void MonitorUtils::launchMonitors() {

	m_cpuMonitor.start();
	m_memoryMonitor.start();
	m_gpuMonitor.start();
	m_diskMonitor.start();
	m_networkMonitor.start();
	m_guiController.start();
}


void MonitorUtils::shutdownMonitors() {
	m_guiController.stop();
	m_networkMonitor.stop();
	m_diskMonitor.stop();
	m_gpuMonitor.stop();
	m_memoryMonitor.stop();
	m_cpuMonitor.stop();
}

