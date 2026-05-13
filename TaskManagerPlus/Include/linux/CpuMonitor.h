#pragma once
#include "MonitorBase.h"

class CpuMonitor : public MonitorBase {

public:

private:

    struct CpuSample {
        double active = 0;
        double total = 0;
    };

    CpuSample m_previousSample;
    CpuSample m_currentSample;

    CpuSample readSample();

    void init() override;

    void monitorLoop() override;
    double computeDelta();

    
    double m_usage = 0;

};
