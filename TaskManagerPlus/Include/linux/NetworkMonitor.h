#pragma once
#include "MonitorBase.h"

class NetworkMonitor : public MonitorBase {

public:
private:

    struct NetworkSample {
        unsigned long rx = 0;
        unsigned long tx = 0;
    };

    NetworkSample m_currentSample;
    NetworkSample m_prevSample;

    void init() override;

    void monitorLoop() override;

    NetworkSample readSample();

    void computeDelta();

    double m_networkUtil = 0.0;
};
