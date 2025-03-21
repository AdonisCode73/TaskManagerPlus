#include "NetworkMonitor.h"

void NetworkMonitor::start() {
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
	if (GetIfTable(nullptr, &ulSize, false) == NO_ERROR) {
		pIfTable = (MIB_IFTABLE*)malloc(ulSize);
		if (pIfTable != NULL && GetIfTable(pIfTable, &ulSize, false) == NO_ERROR) {
			for (DWORD i = 0; i < pIfTable->dwNumEntries; i++) {
				row = pIfTable->table[i];
			}
		}
	}
}

void NetworkMonitor::monitorLoop() {
	MIB_IFROW prevRow = row;
	while (isRunning) {
		DWORD sentBytes = row.dwOutOctets - prevRow.dwOutOctets;
		DWORD recvBytes = row.dwInOctets - prevRow.dwInOctets;
		systemStatus.sendNetwork = (double) sentBytes;
		systemStatus.receiveNetwork = (double) recvBytes;
		prevRow = row;
		getNetworkInfo();
	}
}