#pragma once
#include <curses.h>
#include <unordered_map>
#include <algorithm>
#include "MonitorUtils.h"
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

		void drawBaseLayout(WINDOW* win, const char* title);

		void drawGraphBox(WINDOW* parent, int startY, int startX, int height, int width, const char* title);

		void drawHomePage(WINDOW* win);
		void drawCPUPage(WINDOW* win);
		void drawDiskPage(WINDOW* win);
		void drawGPUPage(WINDOW* win);
		void drawMemoryPage(WINDOW* win);
		void drawNetworkPage(WINDOW* win);

		void renderGraph(WINDOW* win, const std::deque<double>& data, int height, int width, int colourPair);

		void updatePage();

		std::unordered_map<Screen, WINDOW*> m_screenWindows;
		std::unordered_map<Screen, WINDOW*> m_screenGraphBoxes;

		bool m_isRunning = true;
		std::thread m_guiThread;

		int m_rows, m_cols = 0;
		int m_screenIdx = 0;
		int m_maxBars = 0;

		Screen m_currentScreen = HOME;

        std::mutex m_drawMutex;
};