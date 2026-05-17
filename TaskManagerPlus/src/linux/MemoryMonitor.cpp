#include "MemoryMonitor.h"
#include "SystemStatus.h"
#include <fstream>
#include <limits>
#include <iostream>

void MemoryMonitor::init() {
   readSample(); 
}

void MemoryMonitor::monitorLoop(){
    while (m_isRunning) {
        readSample();
        {
            std::lock_guard<std::mutex> lock(systemStatus.memoryMutex);
            systemStatus.checkQueueSize(systemStatus.memoryUsage);
            systemStatus.memoryUsage.push_front(m_memoryUsage);
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void MemoryMonitor::readSample(){
    std::ifstream stat("/proc/meminfo");

    std::string label;
    long availMem;
    long totalMem;

    // line 0: MemTotal:    16295920 kB
    stat >> label >> totalMem;
    systemStatus.ramTotalMemory = (totalMem / 1024.0) / 1024.0; 

    stat.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
    stat.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // line 1: MemAvailable: 10100884   kB
    stat >> label >> availMem;
    systemStatus.ramAvailMemory = (availMem / 1024.0) / 1024.0;

    //std::cout << label << " " << availMem << "\n";

    long usedMemory = totalMem - availMem;
    m_memoryUsage = (static_cast<double>(usedMemory) / static_cast<double>(totalMem)) * 100.0; 
    
    std::cout << m_memoryUsage << std::endl;
}
