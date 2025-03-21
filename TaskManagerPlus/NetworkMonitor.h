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

		bool isRunning;
		std::thread networkThread;

		void monitorLoop();

		void getNetworkInfo();
};
