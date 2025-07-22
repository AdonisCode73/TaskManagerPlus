#pragma once
#include <thread>
#include "CL/cl.h"
#include "ADLXController.h"
#include "NVIDIAController.h"
#include "IGpuController.h"
#include "MonitorBase.h"

class GpuMonitor : public MonitorBase {

public:
	static constexpr cl_uint VENDOR_ID_NVIDIA = 0x10DE;
	static constexpr cl_uint VENDOR_ID_AMD = 0x1002;
	static constexpr cl_uint VENDOR_ID_INTEL = 0x8086; 
	static constexpr cl_uint VENDOR_ID_QUALCOMM = 0x5143;

	inline cl_uint getVendorID() const { return m_vendorID; }

private:
	
	std::unique_ptr<IGpuController> m_controller;

	cl_uint m_vendorID = 0;
	cl_device_id m_deviceID = nullptr;

	void monitorLoop() override;

	void init() override;
	
	void initOpenCL();

	void calculateTotalMem();
};