#pragma once
#include <windows.h>
#include <iphlpapi.h>
#include <thread>
#include "MonitorBase.h"

class NetworkMonitor : public MonitorBase {
	public:

	private:
		MIB_IFROW m_row{};
		MIB_IFROW m_prevRow{};
	
		DWORD m_interfaceIndex = 0;
		bool m_selectedInterfaceIndex = false;

		void monitorLoop() override;

		void init() override;

		void getNetworkInfo();
};
