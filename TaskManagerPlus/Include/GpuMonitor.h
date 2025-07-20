#pragma once
#include "MonitorUtils.h"
#include "CL/cl.h"
#include "ADLXController.h"
#include "NVIDIAController.h"
#include "IGpuController.h"

class GpuMonitor {

public:
	static constexpr cl_uint VENDOR_ID_NVIDIA = 0x10DE;
	static constexpr cl_uint VENDOR_ID_AMD = 0x1002;
	static constexpr cl_uint VENDOR_ID_INTEL = 0x8086; 
	static constexpr cl_uint VENDOR_ID_QUALCOMM = 0x5143;

	void start();

	void stop();

	cl_uint getVendorID() const { return m_vendorID; }

private:
	bool m_isRunning = true;
	std::thread m_gpuThread;
	
	cl_uint m_vendorID = 0;
	cl_device_id m_deviceID = nullptr;

	void monitorLoop(std::unique_ptr<IGpuController> controllerObj);
	
	void initOpenCL();

	void calculateTotalMem();
};