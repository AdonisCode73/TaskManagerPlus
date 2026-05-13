#pragma once
#include "MonitorBase.h"

class GpuMonitor : public MonitorBase {

public:
private:
  void init() override;

  void monitorLoop() override;
};
