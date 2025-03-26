#pragma once
#include <curses.h>
#include <map>
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

		void drawGraphBox(WINDOW* parent, int startY, int startX, int height, int width, const char* title);

		void drawCPUPage(WINDOW* win);
		void drawDiskPage(WINDOW* win);
		void drawGPUPage(WINDOW* win);
		void drawMemoryPage(WINDOW* win);
		void drawNetworkPage(WINDOW* win);

		void renderCPUGraph(WINDOW* win);

		void updatePage();

		std::map<WINDOW*, bool> firstRun;
		std::map<WINDOW*, WINDOW*> windowGraphBox;

		bool isRunning;

		int rows, cols;
		int screenIdx;
		int userInput;

		std::thread guiThread;

		WINDOW* homeW;
		WINDOW* cpuW;
		WINDOW* diskW;
		WINDOW* gpuW;
		WINDOW* memoryW;
		WINDOW* networkW;

		Screen currentScreen;
};