#include "NVIDIAController.h"

#ifdef USE_NVIDIA
void NVIDIAController::initNVML() {
	nvmlReturn_t result = nvmlInit();
	if (result != NVML_SUCCESS) {
		std::cerr << "Failed to initialize NVML: " << std::endl;
		exit(EXIT_FAILURE);
	}
}

void NVIDIAController::update() {
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
	/*std::lock_guard<std::mutex> lock(systemStatus.gpuMutex);*/
	MonitorUtils::checkQueueSize(systemStatus.gpuUsage);
	systemStatus.gpuUsage.push_front((double)utilization.gpu);
	systemStatus.memControllerUsage = utilization.memory;
}
#endif