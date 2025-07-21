#pragma once

#include "CpuMonitor.h"
#include "MemoryMonitor.h"
#include "GpuMonitor.h"
#include "DiskMonitor.h"
#include "NetworkMonitor.h"
#include "GuiController.h"

class MonitorUtils {
public:
	void init();

private:
	void launchMonitors();
	void shutdownMonitors();

	CpuMonitor m_cpuMonitor;
	MemoryMonitor m_memoryMonitor;
	GpuMonitor m_gpuMonitor;
	DiskMonitor m_diskMonitor;
	NetworkMonitor m_networkMonitor;
	GuiController m_guiController;
};