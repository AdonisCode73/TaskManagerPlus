#include "GpuMonitor.h"
#include "SystemStatus.h"
#include <chrono>
#include <thread>

void GpuMonitor::init(){
    readSample();
}

void GpuMonitor::monitorLoop(){
    readSample();
    while (m_isRunning) {
        {
            std::lock_guard<std::mutex> lock(systemStatus.gpuMutex);
            systemStatus.checkQueueSize(systemStatus.gpuUsage);
            systemStatus.gpuUsage.push_front(m_gpuUtil);
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void GpuMonitor::readSample(){
    m_gpuUtil  = readSysfs("/sys/class/drm/card1/device/gpu_busy_percent");
    systemStatus.vramTotalMemory = ((readSysfs("/sys/class/drm/card1/device/mem_info_vram_total") / 1024.0) / 1024.0) / 1024.0;

    long vramUsed = ((readSysfs("/sys/class/drm/card1/device/mem_info_vram_used")  / 1024.0) / 1024.0) / 1024.0;
    systemStatus.vramAvailMemory = systemStatus.vramTotalMemory - vramUsed;

    systemStatus.gpuTemperature = readSysfs("/sys/class/drm/card1/device/hwmon/hwmon2/temp1_input") / 1000.0;

    systemStatus.memControllerUsage    = readSysfs("/sys/class/drm/card1/device/mem_busy_percent"); 
    
    systemStatus.gpuFanSpeed = readSysfs("/sys/class/drm/card1/device/hwmon/hwmon2/fan1_input");

    systemStatus.gpuClockSpeed = readSysfs("/sys/class/drm/card1/device/pp_dpm_mclk");


}

