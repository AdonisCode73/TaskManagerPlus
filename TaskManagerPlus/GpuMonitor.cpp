#include "GpuMonitor.h"
#include "SystemStatus.h"
#include <iostream>

static std::unique_ptr<IGpuController> createGpuController(const cl_uint vendorID);

void GpuMonitor::monitorLoop() {
	while (m_isRunning) {
		m_controller->update();
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

void GpuMonitor::init() {
	initOpenCL();

	m_controller = createGpuController(m_vendorID);
	m_controller->init();
	m_controller->update();
}

void GpuMonitor::initOpenCL() {
	cl_uint platformCount;
	clGetPlatformIDs(0, nullptr, &platformCount);
	cl_platform_id* platforms = new cl_platform_id[platformCount];

	cl_platform_id platformID;
	clGetPlatformIDs(platformCount, platforms, nullptr);
	platformID = platforms[0];
	delete[] platforms;

	cl_uint num_devices;
	clGetDeviceIDs(platformID, CL_DEVICE_TYPE_GPU, 0, nullptr, &num_devices);
	cl_device_id* deviceIDs = new cl_device_id[num_devices];

	clGetDeviceIDs(platformID, CL_DEVICE_TYPE_GPU, num_devices, deviceIDs, nullptr);
	m_deviceID = deviceIDs[0];

	cl_uint vendorID;
	clGetDeviceInfo(m_deviceID, CL_DEVICE_VENDOR_ID, sizeof(vendorID), &vendorID, nullptr);
	m_vendorID = vendorID;

	delete[] deviceIDs;

	calculateTotalMem();
}

void GpuMonitor::calculateTotalMem() {
	cl_ulong totalMemory;
	clGetDeviceInfo(m_deviceID, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(cl_ulong), &totalMemory, nullptr);
	systemStatus.vramTotalMemory = totalMemory / (1024.0 * 1024.0 * 1024.0);
}

static std::unique_ptr<IGpuController> createGpuController(const cl_uint vendorID) {
	std::unique_ptr<IGpuController> controller;

	switch (vendorID) {
	case GpuMonitor::VENDOR_ID_AMD:
		controller = std::make_unique<ADLXController>();
		systemStatus.gpuType = GpuType::AMD;
		break;

	case GpuMonitor::VENDOR_ID_NVIDIA:
		controller = std::make_unique<NVIDIAController>();
		systemStatus.gpuType = GpuType::NVIDIA;
		break;

	case GpuMonitor::VENDOR_ID_INTEL:
		std::cerr << "Intel GPUs currently unsupported";
		exit(1);
		break;

	case GpuMonitor::VENDOR_ID_QUALCOMM:
		std::cerr << "Qualcomm GPUs currently unsupported";
		exit(1);
		break;

	default:
		std::cerr << "Unknown GPU Type!";
		exit(1);
		break;
	}

	 return controller;
}