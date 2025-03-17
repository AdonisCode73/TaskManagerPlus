#pragma once
#pragma once
#include "MonitorUtils.h"
#include <iostream>
#include "CL/cl.h"

class GpuMonitor {


	public:
		void start();

		void stop();


	private:
		cl_uint platformCount;
		cl_uint num_devices;

		cl_ulong totalMemory;
		cl_ulong free_mem[2];

		cl_platform_id platformID;

		cl_device_id deviceID;

		void print_data();

		void calculateTotalMem();

		void calculateAvailMem();
};