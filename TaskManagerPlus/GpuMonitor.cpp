#include "GpuMonitor.h"

static std::unique_ptr<IGpuController> createGpuController();

void GpuMonitor::start() {
	initOpenCL();

	m_isRunning = true;

	std::unique_ptr<IGpuController> controller = createGpuController();
	controller->update();
	m_gpuThread = std::thread(&GpuMonitor::monitorLoop, this, std::move(controller));
}

void GpuMonitor::stop() {
	m_isRunning = false;
	if (m_gpuThread.joinable()) {
		m_gpuThread.join();
	}
}

void GpuMonitor::monitorLoop(std::unique_ptr<IGpuController> controllerObj) {
	while (m_isRunning) {
		controllerObj->update();
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
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
	delete[] deviceIDs;

	calculateTotalMem();
}

void GpuMonitor::calculateTotalMem() {
	cl_ulong totalMemory;
	clGetDeviceInfo(m_deviceID, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(cl_ulong), &totalMemory, nullptr);
	systemStatus.vramTotalMemory = totalMemory/ (1024.0 * 1024.0 * 1024.0);
}

static std::unique_ptr<IGpuController> createGpuController() {
#ifdef USE_AMD
	std::unique_ptr<ADLXController> controller = std::make_unique<ADLXController>();
	controller->initADLX();
	return controller;
#endif
#ifdef USE_NVIDIA
	std::unique_ptr<NVIDIAController> controller = std::make_unique<NVIDIAController>();
	controller->initNVML();
	return controller;
#endif
}