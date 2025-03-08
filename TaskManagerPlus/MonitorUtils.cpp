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
	if (std::cin.get() == '\n') {
		jumpCursor = false;
		return jumpCursor;
	}
	return jumpCursor;
}

void MonitorUtils::stop() {
	cpuMonitor.stop();
	memoryMonitor.stop();
	isRunning = false;

	if (monitorThread.joinable()) {
		monitorThread.join();
	}

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
	std::cout << "You have " << std::fixed << std::setprecision(1) << systemStatus.totalMemory << "GB total memory available in your system.\n";
	std::cout << "You have " << std::fixed << std::setprecision(1) << systemStatus.availMemory << "GB available memory available in your system.\n";

	saveCursorPosition();

	while (isRunning) {
		std::cout << "\r" << std::string(40, ' ') << "\r";
		std::cout << "Current memory utilisation: " << std::fixed << systemStatus.memoryUsage << std::setprecision(1) << "%" << std::flush << std::endl;

		std::cout << "\r" << std::string(30, ' ') << "\r";
		std::cout << "CPU Usage: " << std::fixed << std::setprecision(1) << systemStatus.cpuUsage << " %" << std::flush;
		if (jumpCursor) {
			restoreCursorPosition();
		}
		Sleep(1500);
	}
}

