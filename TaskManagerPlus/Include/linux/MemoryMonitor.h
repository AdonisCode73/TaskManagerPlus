#pragma once
#include "MonitorBase.h"

class MemoryMonitor : public MonitorBase {

public:
private:
  void init() override;

  void monitorLoop() override;
};
