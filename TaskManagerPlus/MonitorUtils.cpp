#include "CpuMonitor.h"
#include "MemoryMonitor.h"
#include "MonitorUtils.h"
#include <windows.h>
#include <iostream>
#include <iomanip>

CpuMonitor cpuMonitor;
MemoryMonitor memoryMonitor;
SystemStatus systemStatus;


void MonitorUtils::start() {
	cpuMonitor.start();
	memoryMonitor.start();


	isRunning = true;
	monitorThread = std::thread(&MonitorUtils::monitorUtil, this);
}

bool MonitorUtils::isRunningCheck() {
	return isRunning;
}

void MonitorUtils::stop() {
	cpuMonitor.stop();
	memoryMonitor.stop();

	if (monitorThread.joinable()) {
		monitorThread.join();
	}

	isRunning = false;
}

void MonitorUtils::saveCursorPosition() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
		initialCursorPos = csbi.dwCursorPosition;
	}
}

void MonitorUtils::restoreCursorPosition() {
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), initialCursorPos);
}

void MonitorUtils::monitorUtil() {
	saveCursorPosition();
	while (isRunning) {
		std::cout << "\r" << std::string(40, ' ') << "\r";
		std::cout << "Current memory utilisation: " << std::fixed << systemStatus.memoryUsage << std::setprecision(1) << "%" << std::flush << std::endl;

		std::cout << "\r" << std::string(30, ' ') << "\r";
		std::cout << "CPU Usage: " << std::fixed << std::setprecision(1) << systemStatus.cpuUsage << " %" << std::flush;
		restoreCursorPosition();
		Sleep(1000);
	}
}

