#pragma once

#include <thread>

class MonitorBase {
public:
	void start() {
		m_isRunning = true;
		init();
		m_thread = std::thread(&MonitorBase::monitorLoop, this);
	}

	void stop() {
		m_isRunning = false;
		if (m_thread.joinable()) {
			m_thread.join();
		}
	}

	virtual ~MonitorBase() = default;

protected:
	bool m_isRunning = false;
	std::thread m_thread;

	virtual void init() = 0;
	virtual void monitorLoop() = 0;
};
