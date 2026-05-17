#include "NetworkMonitor.h"
#include "SystemStatus.h"

void NetworkMonitor::init(){
    m_prevSample = readSample();
}

void NetworkMonitor::monitorLoop(){
    while (m_isRunning) {
        m_currentSample = readSample();
        computeDelta();
        {
            std::lock_guard<std::mutex> lock(systemStatus.networkMutex);
            systemStatus.checkQueueSize(systemStatus.networkUsage);
            systemStatus.networkUsage.push_front(m_networkUtil); 
        }

        m_prevSample = m_currentSample;
    }
}

NetworkMonitor::NetworkSample NetworkMonitor::readSample(){
    unsigned long rx = readSysfs("/sys/class/net/enp42s0/statistics/rx_bytes");
    unsigned long tx = readSysfs("/sys/class/net/enp42s0/statistics/tx_bytes");
    
    NetworkSample sample;
    sample.rx = rx;
    sample.tx = tx; 

    return sample;
}

void NetworkMonitor::computeDelta(){
    unsigned long rxDelta = m_currentSample.rx - m_prevSample.rx;
    unsigned long txDelta = m_currentSample.tx - m_prevSample.tx;

    systemStatus.sendNetwork = (txDelta / 1024.0) / 1024.0;
    systemStatus.receiveNetwork = (rxDelta / 1024.0) / 1024.0;

    m_networkUtil = ((rxDelta + txDelta) * 8.0) / (1000.0 * 1024.0 * 1024.0) * 100;
}
