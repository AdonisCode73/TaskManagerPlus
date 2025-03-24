#include <iostream>
#include "MonitorUtils.h"
#include "GuiController.h"

int main()
{
	MonitorUtils utilMonitor;

	std::cout << "\n**Beginning System Monitoring**\n";

	std::cout << "****Press Enter to exit****\n";
	utilMonitor.start();
	
	while (utilMonitor.isRunningCheck()) {
	}

	utilMonitor.stop();
	std::cout << "\nSafely Exiting application.";
}
