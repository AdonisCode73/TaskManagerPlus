#include "GpuMonitor.h"

void GpuMonitor::start() {
	initOpenCL();
	initADLX();
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

void GpuMonitor::initADLX() {
	ADLX_RESULT  res = ADLX_FAIL;

	res = adlxHelper.Initialize();

	if (ADLX_SUCCEEDED(res)) {
		res = adlxHelper.GetSystemServices()->GetPerformanceMonitoringServices(&perfMonitoringService);
		if (ADLX_SUCCEEDED(res))
		{
			IADLXGPUListPtr gpus;
			res = adlxHelper.GetSystemServices()->GetGPUs(&gpus);
			if (ADLX_SUCCEEDED(res))
			{
				res = gpus->At(gpus->Begin(), &oneGPU);
				if (ADLX_SUCCEEDED(res))
				{
				}
				else {
					std::cout << "Get particular GPU failed" << std::endl;
				}
			}
			else {
				std::cout << "Get GPU list failed" << std::endl;
			}
		}
		else {
			std::cout << "Get performance monitoring services failed" << std::endl;
		}
	}
}
void GpuMonitor::update() {
	perfMonitoringService->GetCurrentGPUMetrics(oneGPU, &gpuMetrics);

	gpuMetrics->GPUVRAM(&usedVram);
	systemStatus.vramAvailMemory = (double) (systemStatus.vramTotalMemory - (usedVram / 1024.0));

	gpuMetrics->GPUVRAMClockSpeed(&vramClockSpeed);
	systemStatus.vramClockSpeed = (double) vramClockSpeed;

	gpuMetrics->GPUTemperature(&gpuTemp);
	systemStatus.gpuTemperature = (double) gpuTemp;

	gpuMetrics->GPUFanSpeed(&fanSpeed);
	systemStatus.gpuFanSpeed = (double) fanSpeed;

	gpuMetrics->GPUUsage(&gpuUsage);
	MonitorUtils::checkQueueSize(systemStatus.gpuUsage);
	systemStatus.gpuUsage.push_front( (double) gpuUsage);
}

void GpuMonitor::monitorLoop() {
	while (isRunning) {
		update();
		Sleep(1000);
	}
}

// 	IADLXGPUList* gpus = nullptr;
// 	system->GetGPUs(&gpus);
// 
// 	IADLXGPU* gpu = nullptr;
// 	gpus->At(0, &gpu); // First GPU
// 
// 	IADLXPerformanceMonitoringServices* perf = nullptr;
// 	system->GetPerformanceMonitoringServices(&perf);
// 
// 	IADLXGPUMetricsPtr* utilization = nullptr;
// 	perf->GetCurrentGPUMetrics(gpu, &utilization);
// 
// 	adlx_int usage = 0;
// 	utilization->GPUUsage(&usage);

// void GpuMonitor::start() {
// 	initNVML();
// 	update();
// 
// 	isRunning = true;
// 	gpuThread = std::thread(&GpuMonitor::monitorLoop, this);
// }
// 
// void GpuMonitor::stop() {
// 	isRunning = false;
// 	if (gpuThread.joinable()) {
// 		gpuThread.join();
// 	}
// }
// 
// void GpuMonitor::initNVML() {
// 	nvmlReturn_t result = nvmlInit();
// 	if (result != NVML_SUCCESS) {
// 		std::cerr << "Failed to initialize NVML: " << std::endl;
// 		exit(EXIT_FAILURE);
// 	}
// }
// 
// void GpuMonitor::update() {
// 	result = nvmlDeviceGetHandleByIndex(0, &device);
// 	if (result != NVML_SUCCESS) {
// 		std::cerr << "Failed to get GPU handle: " << nvmlErrorString(result) << std::endl;
// 		return;
// 	}
// 
// 	result = nvmlDeviceGetMemoryInfo(device, &memInfo);
// 	if (result != NVML_SUCCESS) {
// 		std::cerr << "Failed to get memory info: " << nvmlErrorString(result) << std::endl;
// 		return;
// 	}
// 
// 	result = nvmlDeviceGetUtilizationRates(device, &utilization);
// 	if (result != NVML_SUCCESS) {
// 		std::cerr << "Failed to get GPU utilization: " << nvmlErrorString(result) << std::endl;
// 		return;
// 	}
// 
// 	systemStatus.vramAvailMemory = memInfo.free / (1024.0 * 1024.0 * 1024.0);
// 	systemStatus.vramTotalMemory = memInfo.total / (1024.0 * 1024.0 * 1024.0);
// 	/*std::lock_guard<std::mutex> lock(systemStatus.gpuMutex);*/
// 	MonitorUtils::checkQueueSize(systemStatus.gpuUsage);
// 	systemStatus.gpuUsage.push_front(utilization.gpu);
// 	systemStatus.memControllerUsage = utilization.memory;
// }
// 
// void GpuMonitor::monitorLoop() {
// 	while (isRunning) {
// 		update();
// 		Sleep(1000);
// 	}
// }

/* OpenCL API - probably redundant now*/
/*
void GpuMonitor::start() {
	initOpenCL();

	isRunning = true;
	gpuThread = std::thread(&GpuMonitor::monitorLoop, this);
}

void GpuMonitor::stop() {
	isRunning = false;
	if (gpuThread.joinable()) {
		gpuThread.join();
	}
}
*/

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
	//calculateAvailMem();
}

void GpuMonitor::calculateTotalMem() {
	clGetDeviceInfo(deviceID, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(cl_ulong), &totalMemory, NULL);
	systemStatus.vramTotalMemory = (double) totalMemory / (1024.0 * 1024.0 * 1024.0);
}

/*
void GpuMonitor::calculateAvailMem() {
	clGetDeviceInfo(deviceID, CL_DEVICE_GLOBAL_FREE_MEMORY_AMD, sizeof(cl_ulong), &free_mem, NULL);
}

void GpuMonitor::monitorLoop() {
	while (isRunning) {
		calculateTotalMem();
		calculateAvailMem();
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}*/