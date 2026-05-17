#include "CpuMonitor.h"
#include "SystemStatus.h"
#include <fstream>

void CpuMonitor::init(){ 
    m_previousSample = readSample();
}

void CpuMonitor::monitorLoop(){
	while (m_isRunning) {

        m_currentSample = readSample();
        m_usage = computeDelta();
		{
			std::lock_guard<std::mutex> lock(systemStatus.cpuMutex);
            systemStatus.checkQueueSize(systemStatus.cpuUsage);
			systemStatus.cpuUsage.push_front(m_usage);
		}
        m_previousSample = m_currentSample;
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

CpuMonitor::CpuSample CpuMonitor::readSample(){

    std::ifstream stats("/proc/stat");
    
    std::string label;
    long user, nice, sys, idle, iowait, irq, softirq;
    stats >> label >> user >> nice >> sys >> idle >> iowait >> irq >> softirq;

    CpuSample sample;
    sample.active = user + nice + sys + irq + softirq;
    sample.total = sample.active + idle + iowait;

    return sample;
}

double CpuMonitor::computeDelta(){
    double activeDelta = m_currentSample.active - m_previousSample.active;
    double totalDelta = m_currentSample.total - m_previousSample.total;

    return (activeDelta * 100) / totalDelta;
}
