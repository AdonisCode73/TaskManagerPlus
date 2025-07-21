#pragma once

#include "IMonitor.h"
#include <thread>

class MonitorBase : public IMonitor {
public:
	void start() override {
		m_isRunning = true;
		init();
		m_thread = std::thread(&MonitorBase::monitorLoop, this);
	}

	void stop() override {
		m_isRunning = false;
		if (m_thread.joinable()) {
			m_thread.join();
		}
	}

protected:
	bool m_isRunning = false;
	std::thread m_thread;

	virtual void init() = 0;
	virtual void monitorLoop() = 0;
};
