#pragma once
#include "MonitorUtils.h"
//#include "CL/cl.h"
#include "nvml.h"

class GpuMonitor {

	public:
		void start();

		void stop();

	private:
		nvmlDevice_t device;
		nvmlReturn_t result;
		nvmlMemory_t memInfo;
		nvmlUtilization_t utilization;

		bool isRunning;
		std::thread gpuThread;

		void initNVML();


		void monitorLoop();

		void update();
		
		/*
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

		void calculateAvailMem();
		*/
};