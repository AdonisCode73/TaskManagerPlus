#pragma once
#include "MonitorBase.h"

class GpuMonitor : public MonitorBase {

public:
private:
    void init() override;

    void monitorLoop() override;

    void readSample();

    double m_gpuUtil = 0.0;
};
