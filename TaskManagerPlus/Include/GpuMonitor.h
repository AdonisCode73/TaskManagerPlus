#pragma once
#include "MonitorUtils.h"
#include "CL/cl.h"
#include "ADLXController.h"
#include "NVIDIAController.h"
#include "IGpuController.h"

class GpuMonitor {

	public:
		void start();

		void stop();

	private:
		bool m_isRunning = true;
		std::thread m_gpuThread;
		
		cl_device_id m_deviceID = nullptr;
	
		void monitorLoop(std::unique_ptr<IGpuController> controllerObj);
		
		void initOpenCL();

		void calculateTotalMem();
};