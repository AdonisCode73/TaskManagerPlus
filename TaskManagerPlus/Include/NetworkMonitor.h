#pragma once
#include "MonitorUtils.h"
#include <iphlpapi.h>

class NetworkMonitor {
	public:
		void start();

		void stop();


	private:
		MIB_IFROW m_row{};
		MIB_IFROW m_prevRow{};
	
		DWORD m_interfaceIndex = 0;
		bool m_selectedInterfaceIndex = false;

		bool m_isRunning = true;
		std::thread m_networkThread;

		void monitorLoop();

		void getNetworkInfo();
};
