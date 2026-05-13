#pragma once
#include "MonitorBase.h"

class NetworkMonitor : public MonitorBase {

public:
private:
  void init() override;

  void monitorLoop() override;
};
