#include "NetworkMonitor.h"
#include "SystemStatus.h"
#include <mutex>

void NetworkMonitor::init() {
	getNetworkInfo();
}

void NetworkMonitor::getNetworkInfo() {
	ULONG ulSize = 0;
	if (GetIfTable(nullptr, &ulSize, false) != ERROR_INSUFFICIENT_BUFFER) {
		return;
	}

	MIB_IFTABLE* pIfTable = (MIB_IFTABLE*)malloc(ulSize);

	if (!pIfTable) {
		return;
	}

	if (pIfTable && GetIfTable(pIfTable, &ulSize, false) != NO_ERROR) {
		free(pIfTable);
		return;
	}

	if (!m_selectedInterfaceIndex) {
		for (DWORD i = 0; i < pIfTable->dwNumEntries; i++) {
			MIB_IFROW tempRow;
			tempRow = pIfTable->table[i];

			if (tempRow.dwOperStatus == IF_OPER_STATUS_OPERATIONAL &&
				tempRow.dwType != IF_TYPE_SOFTWARE_LOOPBACK &&
				tempRow.dwSpeed > 0 &&
				(tempRow.dwOutOctets > 0 || tempRow.dwInOctets > 0)) {

				m_interfaceIndex = tempRow.dwIndex;
				m_selectedInterfaceIndex = true;
				m_row = tempRow;
				return;
			}
		}
	}
	else {
		for (DWORD i = 0; i < pIfTable->dwNumEntries; i++) {
			if (pIfTable->table[i].dwIndex == m_interfaceIndex) {
				m_row = pIfTable->table[i];
				return;
			}
		}
	}
	free(pIfTable);
}

void NetworkMonitor::monitorLoop() {
	getNetworkInfo();
	m_prevRow = m_row;

	while (m_isRunning) {
		getNetworkInfo();

		DWORD sentBytes = m_row.dwOutOctets - m_prevRow.dwOutOctets;
		DWORD recvBytes = m_row.dwInOctets - m_prevRow.dwInOctets;
		systemStatus.sendNetwork = (sentBytes * 8.0) / 1000.0;
		systemStatus.receiveNetwork = (recvBytes * 8.0) / 1000.0;

		double networkUtil = (((sentBytes + recvBytes) * 8.0) / static_cast<double>(m_row.dwSpeed));
		{
			std::lock_guard<std::mutex> lock(systemStatus.networkMutex);
			systemStatus.checkQueueSize(systemStatus.networkUsage);
			systemStatus.networkUsage.push_front(networkUtil * 100.0);
		}
		m_prevRow = m_row;
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}