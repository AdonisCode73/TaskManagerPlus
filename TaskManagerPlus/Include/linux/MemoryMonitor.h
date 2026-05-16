#pragma once
#include "MonitorBase.h"

class MemoryMonitor : public MonitorBase {

public:
private:
    void init() override;

    void monitorLoop() override;

    void readSample();

    long m_totalMemory = 0;
    long m_availMemory = 0;

    double m_memoryUsage = 0;
};
