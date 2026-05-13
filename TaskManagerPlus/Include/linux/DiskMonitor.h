#pragma once
#include "MonitorBase.h"

class DiskMonitor : public MonitorBase {

public:
private:
  void init() override;

  void monitorLoop() override;
};
