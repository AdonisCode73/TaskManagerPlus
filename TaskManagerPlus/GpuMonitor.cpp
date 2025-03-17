#include "GpuMonitor.h"

void GpuMonitor::start() {
	initNVML();
	update();

	isRunning = true;
	gpuThread = std::thread(&GpuMonitor::monitorLoop, this);
}

void GpuMonitor::stop() {
	isRunning = false;
	if (gpuThread.joinable()) {
		gpuThread.join();
	}
}

void GpuMonitor::initNVML() {
	nvmlReturn_t result = nvmlInit();
	if (result != NVML_SUCCESS) {
		std::cerr << "Failed to initialize NVML: " << std::endl;
		exit(EXIT_FAILURE);
	}
}

void GpuMonitor::update() {
	result = nvmlDeviceGetHandleByIndex(0, &device);
	if (result != NVML_SUCCESS) {
		std::cerr << "Failed to get GPU handle: " << nvmlErrorString(result) << std::endl;
		return;
	}

	result = nvmlDeviceGetMemoryInfo(device, &memInfo);
	if (result != NVML_SUCCESS) {
		std::cerr << "Failed to get memory info: " << nvmlErrorString(result) << std::endl;
		return;
	}

	result = nvmlDeviceGetUtilizationRates(device, &utilization);
	if (result != NVML_SUCCESS) {
		std::cerr << "Failed to get GPU utilization: " << nvmlErrorString(result) << std::endl;
		return;
	}

	systemStatus.vramAvailMemory = memInfo.free / (1024.0 * 1024.0 * 1024.0);
	systemStatus.vramTotalMemory = memInfo.total / (1024.0 * 1024.0 * 1024.0);
	systemStatus.gpuUsage = utilization.gpu;  
	systemStatus.memControllerUsage = utilization.memory;
}

void GpuMonitor::monitorLoop() {
	while (isRunning) {
		update();
	}
}

/* OPENCL API - MAY REUSE THIS CODE LATER IF DEFINING AMD-GPU FOR GPU MONITORING
void GpuMonitor::start() {
	clGetPlatformIDs(0, nullptr, &platformCount);
	cl_platform_id* platforms = new cl_platform_id[platformCount];

	clGetPlatformIDs(platformCount, platforms, nullptr);
	platformID = platforms[0];
	delete[] platforms;

	clGetDeviceIDs(platformID, CL_DEVICE_TYPE_GPU, 0, nullptr, &num_devices);
	cl_device_id* deviceIDs = new cl_device_id[num_devices];

	clGetDeviceIDs(platformID, CL_DEVICE_TYPE_GPU, num_devices, deviceIDs, nullptr);
	deviceID = deviceIDs[0];
	delete[] deviceIDs;

	calculateTotalMem();

	print_data();
}

void GpuMonitor::calculateTotalMem() {
	clGetDeviceInfo(deviceID, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(cl_ulong), &totalMemory, NULL);
	systemStatus.vramTotalMemory = (double) totalMemory / (1024.0 * 1024.0 * 1024.0);
}

void GpuMonitor::calculateAvailMem() {
	//clGetDeviceInfo(deviceID, CL_DEVICE_GLOBAL_FREE_MEMORY_AMD, sizeof(cl_ulong), &free_mem, NULL);
}

void GpuMonitor::print_data() {
	std::cout << platformID << std::endl;
	std::cout << deviceID << std::endl;
	std::cout << num_devices << std::endl;
}*/