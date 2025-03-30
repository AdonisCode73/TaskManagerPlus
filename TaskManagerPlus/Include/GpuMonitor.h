#pragma once
#include "MonitorUtils.h"
#include "CL/cl.h"
//#include "nvml.h"
#include "ADLXHelper.h"
#include "IPerformanceMonitoring.h"
#include "IPerformanceMonitoring2.h"



class GpuMonitor {

	public:
		void start();

		void stop();

	private:
		ADLXHelper adlxHelper;

		IADLXGPUMetricsPtr gpuMetrics;
		IADLXGPUMetricsSupportPtr gpuMetricsSupport;

		adlx_double gpuUsage;
		adlx_int usedVram;
		adlx_int gpuClockSpeed;
		adlx_double gpuTemp;
		adlx_int fanSpeed;

		IADLXPerformanceMonitoringServicesPtr perfMonitoringService;
		IADLXGPUPtr oneGPU;

// 		nvmlDevice_t device;
// 		nvmlReturn_t result;
// 		nvmlMemory_t memInfo;
// 		nvmlUtilization_t utilization;

		bool isRunning;
		std::thread gpuThread;

		//void initNVML();

		void initADLX();

		void monitorLoop();

		void update();
		
		
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

		//void calculateAvailMem();
		
};