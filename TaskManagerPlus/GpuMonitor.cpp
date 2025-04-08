#include "GpuMonitor.h"

void GpuMonitor::start() {
	initOpenCL();

	isRunning = true;
#ifdef USE_AMD
	auto* adlxController = new ADLXController();
	adlxController->initADLX();
	adlxController->update();
	gpuThread = std::thread(&GpuMonitor::monitorLoop<ADLXController>, this, adlxController);
#else
	auto* nvidiaController = new NVIDIAController();
	nvidiaController->initNVML();
	nvidiaController->update();
	gpuThread = std::thread(&GpuMonitor::monitorLoop<NVIDIAController>, this, nvidiaController);
#endif
}

void GpuMonitor::stop() {
	isRunning = false;
	if (gpuThread.joinable()) {
		gpuThread.join();
	}
}
template <typename T>
void GpuMonitor::monitorLoop(T* controllerObj) {
	while (isRunning) {
		controllerObj->update();
		Sleep(1000);
	}
}

void GpuMonitor::initOpenCL() {
	clGetPlatformIDs(0, nullptr, &platformCount);
	platforms = new cl_platform_id[platformCount];

	clGetPlatformIDs(platformCount, platforms, nullptr);
	platformID = platforms[0];
	delete[] platforms;

	clGetDeviceIDs(platformID, CL_DEVICE_TYPE_GPU, 0, nullptr, &num_devices);
	deviceIDs = new cl_device_id[num_devices];

	clGetDeviceIDs(platformID, CL_DEVICE_TYPE_GPU, num_devices, deviceIDs, nullptr);
	deviceID = deviceIDs[0];
	delete[] deviceIDs;

	calculateTotalMem();
}

void GpuMonitor::calculateTotalMem() {
	clGetDeviceInfo(deviceID, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(cl_ulong), &totalMemory, NULL);
	systemStatus.vramTotalMemory = (double) totalMemory / (1024.0 * 1024.0 * 1024.0);
}