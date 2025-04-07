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
		bool isRunning;
		std::thread gpuThread;

		template <typename T>	
		void monitorLoop(T* controllerObj);
		
		void initOpenCL();
		
		cl_uint platformCount;
		cl_uint num_devices;

		cl_ulong totalMemory;
		cl_ulong free_mem[2];

		cl_platform_id platformID;

		cl_device_id deviceID;

		cl_platform_id* platforms;
		cl_device_id* deviceIDs;

		void calculateTotalMem();
};