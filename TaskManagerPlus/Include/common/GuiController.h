#pragma once
#include <curses.h>
#include <unordered_map>
#include <algorithm>
#include <deque>
#include <thread>
#include <mutex>

class GuiController {

public:

	static constexpr int SCREEN_COUNT = 6;

	void start();
	void stop();

protected:

	enum class Screen {
		HOME,
		CPU,
		DISK,
		GPU,
		MEMORY,
		NETWORK
	};

private:
	enum class HeaderType {
		Main,
		Sub
	};

	void guiInit();

	void updatePage();
	void navigateWindows();
	void changeWindow();

	const char* getHeader(HeaderType type) const;

	void drawBaseLayout(WINDOW* win, const char* header);
	void drawPage(WINDOW* win, Screen screen, const char* header);

	void drawGraphBox(WINDOW* parent, int startY, int startX, int height, int width, const char* title);
	void renderGraph(WINDOW* win, const std::deque<double>& data, int height, int colourPair);

	void guiShutdown();

	std::unordered_map<Screen, WINDOW*> m_screenWindows;
	std::unordered_map<Screen, WINDOW*> m_screenGraphBoxes;

	int m_rows = 0, m_cols = 0;
	int m_screenIdx = 0;
	int m_maxBars = 0;

	bool m_isRunning = true;
	std::thread m_guiThread;

	Screen m_currentScreen = Screen::HOME;

	std::mutex m_drawMutex;
};