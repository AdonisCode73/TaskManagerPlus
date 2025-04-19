#include "NVIDIAController.h"

#ifdef USE_NVIDIA
void NVIDIAController::initNVML() {
	m_result = nvmlInit();
	if (m_result != NVML_SUCCESS) {
		std::cerr << "Failed to initialize NVML: " << std::endl;
		exit(EXIT_FAILURE);
	}
}

void NVIDIAController::update() {
	nvmlDevice_t device;
	m_result = nvmlDeviceGetHandleByIndex(0, &device);
	if (m_result != NVML_SUCCESS) {
		std::cerr << "Failed to get GPU handle: " << nvmlErrorString(m_result) << std::endl;
		return;
	}

	nvmlMemory_t memInfo;
	m_result = nvmlDeviceGetMemoryInfo(device, &memInfo);
	if (m_result != NVML_SUCCESS) {
		std::cerr << "Failed to get memory info: " << nvmlErrorString(m_result) << std::endl;
		return;
	}

	nvmlUtilization_t utilization;
	m_result = nvmlDeviceGetUtilizationRates(device, &utilization);
	if (m_result != NVML_SUCCESS) {
		std::cerr << "Failed to get GPU utilization: " << nvmlErrorString(m_result) << std::endl;
		return;
	}

	systemStatus.vramAvailMemory = memInfo.free / (1024.0 * 1024.0 * 1024.0);
	/*std::lock_guard<std::mutex> lock(systemStatus.gpuMutex);*/
	MonitorUtils::checkQueueSize(systemStatus.gpuUsage);
	systemStatus.gpuUsage.push_front((double)utilization.gpu);
	systemStatus.memControllerUsage = utilization.memory;
}
#endif