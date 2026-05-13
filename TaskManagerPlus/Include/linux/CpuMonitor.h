#pragma once
#include "MonitorBase.h"

class CpuMonitor : MonitorBase {

public:
private:
  void init() override;

  void monitorLoop() override;
};
