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

	//isRunning = true;
	//monitorThread = std::thread(&MonitorUtils::monitorUtil, this);
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
	gpuMonitor.stop();
	diskMonitor.stop();
	networkMonitor.stop();
	isRunning = false;

	if (monitorThread.joinable()) {
		monitorThread.join();
	}
}

void MonitorUtils::checkQueueSize(std::queue <double> statQueue) {
	if (statQueue.size() == MAX_QUEUE_SIZE) {
		statQueue.pop();
	}
}

//void MonitorUtils::saveCursorPosition() {
//	CONSOLE_SCREEN_BUFFER_INFO csbi;
//	if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
//		initialCursorPos = csbi.dwCursorPosition;
//	}
//}
//
//void MonitorUtils::restoreCursorPosition() {
//	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), initialCursorPos);
//}

void MonitorUtils::monitorUtil() {
	/*std::cout << "\nRAM:\n";
	std::cout << "You have " << std::fixed << std::setprecision(1) << systemStatus.ramTotalMemory << "GB total memory available in your system.\n";
	std::cout << "You have " << std::fixed << std::setprecision(1) << systemStatus.ramAvailMemory << "GB available memory in your system.\n";*/

	/*std::cout << "\nVRAM:\n";
	std::cout << "You have " << std::fixed << std::setprecision(1) << systemStatus.vramTotalMemory << "GB total memory available in your system.\n";
	std::cout << "You have " << std::fixed << std::setprecision(1) << systemStatus.vramAvailMemory << "GB available memory in your system.\n";*/

	/*std::cout << "\nDISK:\n";
	std::cout << "Total Space: " << systemStatus.totalDisk << " GB total memory available in your system.\n";
	std::cout << "Free Space: " << systemStatus.availDisk << " GB available memory in your system.\n";*/

	/*std::cout << "\nCurrent utilisation:\n";*/
	/*saveCursorPosition();*/

	while (isRunning) {
		/*std::cout << "\r" << std::string(40, ' ') << "\r";
		std::cout << "Current memory utilisation: " << std::fixed << systemStatus.memoryUsage << std::setprecision(1) << "%" << std::flush << std::endl;*/

		/*std::cout << "\r" << std::string(40, ' ') << "\r";
		std::cout << "CPU Usage: " << std::fixed << std::setprecision(1) << systemStatus.cpuUsage << " %" << std::flush << std::endl;*/

		/*std::cout << "\r" << std::string(40, ' ') << "\r";
		std::cout << "GPU Usage: " << std::fixed << std::setprecision(1) << systemStatus.gpuUsage << " %" << std::flush << std::endl;

		std::cout << "\r" << std::string(40, ' ') << "\r";
		std::cout << "Memory Controller Usage: " << std::fixed << std::setprecision(1) << systemStatus.memControllerUsage << " %" << std::flush << std::endl;*/

		/*std::cout << "\r" << std::string(40, ' ') << "\r";
		std::cout << "Disk Read Usage: " << std::fixed << std::setprecision(1) << systemStatus.readDisk << "KB/s" << std::flush << std::endl;

		std::cout << "\r" << std::string(40, ' ') << "\r";
		std::cout << "Disk Write Usage: " << std::fixed << std::setprecision(1) << systemStatus.writeDisk << "KB/s" << std::flush << std::endl;

		std::cout << "\r" << std::string(40, ' ') << "\r";
		std::cout << "Disk Utilisation: " << std::fixed << std::setprecision(1) << systemStatus.diskTime << " %" << std::flush << std::endl;*/

		/*std::cout << "\r" << std::string(40, ' ') << "\r";
		std::cout << "Network Bytes Sent: " << std::fixed << std::setprecision(1) << systemStatus.sendNetwork << "Kbps" << std::flush << std::endl;

		std::cout << "\r" << std::string(40, ' ') << "\r";
		std::cout << "Network Bytes Received: " << std::fixed << std::setprecision(1) << systemStatus.receiveNetwork << "Kbps" << std::flush << std::endl;*/
		/*if (jumpCursor) {
			restoreCursorPosition();
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));*/
	}
}

