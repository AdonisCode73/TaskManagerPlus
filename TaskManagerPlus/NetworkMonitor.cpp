#include "NetworkMonitor.h"

void NetworkMonitor::start() {
	selectedInterfaceIndex = false;
	interfaceIndex = 0;

	getNetworkInfo();

	isRunning = true;
	networkThread = std::thread(&NetworkMonitor::monitorLoop, this);
}

void NetworkMonitor::stop() {
	isRunning = false;
	if (networkThread.joinable()) {
		networkThread.join();
	}
}

void NetworkMonitor::getNetworkInfo() {
	if (GetIfTable(nullptr, &ulSize, false) == ERROR_INSUFFICIENT_BUFFER) {
		pIfTable = (MIB_IFTABLE*)malloc(ulSize);
		if (pIfTable && GetIfTable(pIfTable, &ulSize, false) == NO_ERROR) {

			if (!selectedInterfaceIndex) {
				for (DWORD i = 0; i < pIfTable->dwNumEntries; i++) {
					tempRow = pIfTable->table[i];

					if (tempRow.dwOperStatus == IF_OPER_STATUS_OPERATIONAL &&
						tempRow.dwType != IF_TYPE_SOFTWARE_LOOPBACK &&
						tempRow.dwSpeed > 0) {

						interfaceIndex = tempRow.dwIndex;
						selectedInterfaceIndex = true;
						row = tempRow; 
						break;
					}
				}
			}
			else {
				for (DWORD i = 0; i < pIfTable->dwNumEntries; i++) {
					if (pIfTable->table[i].dwIndex == interfaceIndex) {
						row = pIfTable->table[i];
						break;
					}
				}
			}
		}
	}
}

void NetworkMonitor::monitorLoop() {
	getNetworkInfo();
	prevRow = row;

	while (isRunning) {
		getNetworkInfo();

		sentBytes = row.dwOutOctets - prevRow.dwOutOctets;
		recvBytes = row.dwInOctets - prevRow.dwInOctets;
		systemStatus.sendNetwork = (sentBytes * 8.0) / 1000.0;
		systemStatus.receiveNetwork = (recvBytes * 8.0) / 1000.0;

		prevRow = row;
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}