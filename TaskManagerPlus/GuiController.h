#pragma once
#include <curses.h>
#include "MonitorUtils.h";

#define NUM_WINDOWS 6

enum Screen {
	HOME, 
	CPU, 
	DISK, 
	GPU, 
	MEMORY, 
	NETWORK 
};

class GuiController {

	public:
		void start();

		void stop();

	private:
		void guiInit();

		void navigateWindows();

		void changeWindow();

		void drawBaseLayout(WINDOW* win, const char* title, const char* footer);

		void drawCPUPage(WINDOW* win);
		void drawDiskPage(WINDOW* win);
		void drawGPUPage(WINDOW* win);
		void drawMemoryPage(WINDOW* win);
		void drawNetworkPage(WINDOW* win);

		void updatePage();

		bool isRunning;

		int rows, cols;
		int screenIdx;
		int userInput;

		const char* underline = "- - -";

		std::thread guiThread;

		WINDOW* homeW;
		WINDOW* cpuW;
		WINDOW* diskW;
		WINDOW* gpuW;
		WINDOW* memoryW;
		WINDOW* networkW;

		Screen currentScreen;
};