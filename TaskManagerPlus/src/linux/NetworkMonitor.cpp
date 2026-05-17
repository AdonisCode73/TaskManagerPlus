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
        std::this_thread::sleep_for(std::chrono::seconds(1));
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
    unsigned long linkSpeed = readSysfs("/sys/class/net/enp42s0/speed"); // Mbps

    double linkSpeedBps = linkSpeed * 1000.0 * 1000.0;
    double deltaBits = (rxDelta + txDelta) * 8.0;

    systemStatus.sendNetwork = (txDelta * 8) / 1000.0;
    systemStatus.receiveNetwork = (rxDelta * 8) / 1000.0;

    m_networkUtil = (deltaBits / linkSpeedBps) * 100.0;
}
