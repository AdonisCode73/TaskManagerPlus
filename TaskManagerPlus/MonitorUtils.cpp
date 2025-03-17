#include "CpuMonitor.h"
#include "MemoryMonitor.h"
#include "MonitorUtils.h"
#include "GpuMonitor.h"


CpuMonitor cpuMonitor;
MemoryMonitor memoryMonitor;
GpuMonitor gpuMonitor;
SystemStatus systemStatus;


void MonitorUtils::start() {
	cpuMonitor.start();
	memoryMonitor.start();
	gpuMonitor.start();

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
	std::cout << "RAM:\n";
	std::cout << "You have " << std::fixed << std::setprecision(1) << systemStatus.ramTotalMemory << "GB total memory available in your system.\n";
	std::cout << "You have " << std::fixed << std::setprecision(1) << systemStatus.ramAvailMemory << "GB available memory in your system.\n";
	
	std::cout << "VRAM:\n";
	std::cout << "You have " << std::fixed << std::setprecision(1) << systemStatus.vramTotalMemory << "GB total memory available in your system.\n";

	saveCursorPosition();

	while (isRunning) {
		std::cout << "\r" << std::string(40, ' ') << "\r";
		std::cout << "Current memory utilisation: " << std::fixed << systemStatus.memoryUsage << std::setprecision(1) << "%" << std::flush << std::endl;

		std::cout << "\r" << std::string(30, ' ') << "\r";
		std::cout << "CPU Usage: " << std::fixed << std::setprecision(1) << systemStatus.cpuUsage << " %" << std::flush << std::endl;
		if (jumpCursor) {
			restoreCursorPosition();
		}
		Sleep(1500);
	}
}

