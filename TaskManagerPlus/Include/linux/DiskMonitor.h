#pragma once
#include "MonitorBase.h"

class DiskMonitor : public MonitorBase {

public:
private:

    struct DiskSample{
        long sectorsRead = 0;
        long sectorsWritten = 0;
        long ioTime = 0;
    };

    void init() override;

    void monitorLoop() override;

    void computeDelta();

    DiskSample readSample();

    double m_diskUtil;

    DiskSample m_prevSample;
    DiskSample m_currentSample;
};
