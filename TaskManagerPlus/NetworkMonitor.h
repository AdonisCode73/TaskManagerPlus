#pragma once
#include "MonitorUtils.h"
#include <iphlpapi.h>

class NetworkMonitor {
	public:
		void start();

		void stop();


	private:
		MIB_IFTABLE* pIfTable;
		ULONG ulSize;
		MIB_IFROW row;
		MIB_IFROW prevRow;
		MIB_IFROW tempRow;
		DWORD interfaceIndex;
		bool selectedInterfaceIndex;

		DWORD sentBytes;
		DWORD recvBytes;

		bool isRunning;
		std::thread networkThread;

		void monitorLoop();

		void getNetworkInfo();
};
