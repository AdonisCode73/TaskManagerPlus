#pragma once

class IGpuController {
	public:
		virtual void update() = 0;
		virtual void init() = 0;
};
