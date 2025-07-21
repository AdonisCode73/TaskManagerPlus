#pragma once

class IMonitor {
public:
	virtual void start() = 0;
	virtual void stop() = 0;
	virtual ~IMonitor() = default;
};