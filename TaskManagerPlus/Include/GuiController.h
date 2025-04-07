#pragma once
#include <curses.h>
#include <map>
#include "MonitorUtils.h";

#define NUM_WINDOWS 6

#define USE_AMD
//#define USE_NVIDIA

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

		void drawHomePage(WINDOW* win);
		void drawCPUPage(WINDOW* win);
		void drawDiskPage(WINDOW* win);
		void drawGPUPage(WINDOW* win);
		void drawMemoryPage(WINDOW* win);
		void drawNetworkPage(WINDOW* win);

		void renderCPUGraph(WINDOW* win, int height, int width);
		void renderDiskGraph(WINDOW* win, int height, int width);
		void renderGPUGraph(WINDOW* win, int height, int width);
		void renderMemoryGraph(WINDOW* win, int height, int width);
		void renderNetworkGraph(WINDOW* win, int height, int width);

		void updatePage();

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