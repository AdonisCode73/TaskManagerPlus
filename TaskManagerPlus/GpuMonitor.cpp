#include "GpuMonitor.h"

void GpuMonitor::start() {
	initOpenCL();

	m_isRunning = true;
#ifdef USE_AMD
	ADLXController* adlxController = new ADLXController();
	adlxController->initADLX();
	adlxController->update();
	m_gpuThread = std::thread(&GpuMonitor::monitorLoop<ADLXController>, this, adlxController);
#endif
#ifdef USE_NVIDIA
	NVIDIAController* nvidiaController = new NVIDIAController();
	nvidiaController->initNVML();
	nvidiaController->update();
	m_gpuThread = std::thread(&GpuMonitor::monitorLoop<NVIDIAController>, this, nvidiaController);
#endif
}

void GpuMonitor::stop() {
	m_isRunning = false;
	if (m_gpuThread.joinable()) {
		m_gpuThread.join();
	}
}
template <typename T>
void GpuMonitor::monitorLoop(T* controllerObj) {
	while (m_isRunning) {
		controllerObj->update();
		Sleep(1000);
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
	systemStatus.vramTotalMemory = (double) totalMemory / (1024.0 * 1024.0 * 1024.0);
}