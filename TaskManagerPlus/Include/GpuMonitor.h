#pragma once
#include "MonitorUtils.h"
#include "CL/cl.h"
#include "ADLXController.h"
#include "NVIDIAController.h"

class GpuMonitor {

	public:
		void start();

		void stop();

	private:
		bool m_isRunning;
		std::thread m_gpuThread;
		
		cl_device_id m_deviceID;

		template <typename T>	
		void monitorLoop(T* controllerObj);
		
		void initOpenCL();

		void calculateTotalMem();
};