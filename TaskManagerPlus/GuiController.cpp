#include "GuiController.h"
#include "SystemStatus.h"

void GuiController::guiInit() {
	initscr();
	noecho();
	curs_set(FALSE);
	keypad(stdscr, TRUE);

	getmaxyx(stdscr, m_rows, m_cols);

	for (int i = 0; i < SCREEN_COUNT; i++) {
		Screen screen = static_cast<Screen>(i);
		m_screenWindows[screen] = newwin(m_rows, m_cols, 0, 0);
	}

	start_color();
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	init_pair(2, COLOR_YELLOW, COLOR_BLACK);
	init_pair(3, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(4, COLOR_CYAN, COLOR_BLACK);
	init_pair(5, COLOR_RED, COLOR_BLACK);

	clear();
}

void GuiController::start() {
	guiInit();

	m_guiThread = std::thread(&GuiController::updatePage, this);

	navigateWindows();

	guiShutdown();
}

void GuiController::stop() {
	m_isRunning = false;
	if (m_guiThread.joinable()) {
		m_guiThread.join();
	}
}

void GuiController::guiShutdown() {
	endwin();
	systemStatus.shutdownFlag = true;
}

void GuiController::navigateWindows() {
	changeWindow();
	int userInput;
	while ((userInput = getch()) != '\n') {
		switch (userInput) {
		case KEY_RESIZE: {
			std::lock_guard<std::mutex> lock(m_drawMutex);
			resize_term(0, 0);
			getmaxyx(stdscr, m_rows, m_cols);

			for (auto& pair : m_screenGraphBoxes) {
				if (pair.second) {
					delwin(pair.second);
				}
			}

			m_screenGraphBoxes.clear();

			for (auto& pair : m_screenWindows) {
				if (pair.second) {
					delwin(pair.second);
				}
				pair.second = newwin(m_rows, m_cols, 0, 0);
			}

			clear();
			refresh();
			changeWindow();
			break;
		}
		case KEY_LEFT:
			m_screenIdx = (m_screenIdx == 0) ? SCREEN_COUNT - 1 : m_screenIdx - 1;
			m_currentScreen = static_cast<Screen>(m_screenIdx);
			changeWindow();
			break;
		case KEY_RIGHT:
			m_screenIdx = (m_screenIdx == SCREEN_COUNT - 1) ? 0 : m_screenIdx + 1;
			m_currentScreen = static_cast<Screen>(m_screenIdx);
			changeWindow();
			break;
		case '\n':
			return;
		default:
			break;
		}
	}
}

void GuiController::changeWindow() {
	WINDOW* win = m_screenWindows[m_currentScreen];

	drawBaseLayout(win, getHeader(HeaderType::Main));

	drawPage(win, m_currentScreen, getHeader(HeaderType::Sub));
}

void GuiController::updatePage() {
	while (m_isRunning) {
		{
			std::lock_guard<std::mutex> lock(m_drawMutex);
			changeWindow();
			refresh();
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

void GuiController::drawPage(WINDOW* win, Screen screen, const char* header) {
	mvwprintw(win, (m_rows / 2) - 2, 2, "%s", header);

	switch (screen) {
	case Screen::HOME: {
		const char* banner[] = { " _____         _      __  __                                   ",
		"|_   _|_ _ ___| | __ |  \\/  | __ _ _ __   __ _  __ _  ___ _ __ ",
		"  | |/ _` / __| |/ / | |\\/| |/ _` | '_ \\ / _` |/ _` |/ _ \\ '__|",
		"  | | (_| \\__ \\   <  | |  | | (_| | | | | (_| | (_| |  __/ |   ",
		" _|_|\\__,_|___/_|\\_\\ |_|  |_|\\__,_|_| |_|\\__,_|\\__, |\\___|_|   ",
		"|  _ \\| |_   _ ___                             |___/          ",
		"| |_) | | | | / __|                                            ",
		"|  __/| | |_| \\__ \\                                            ",
		"|_|   |_|\\__,_|___/                                            "
		};

		int numLines = sizeof(banner) / sizeof(banner[0]);

		for (int i = 0; i < numLines; ++i) {
			mvwprintw(win, ((m_rows - numLines) / 2) + i, (m_cols - static_cast<int>(strlen(banner[0]))) / 2, "%s", banner[i]);
		}
		break;
	}

	case Screen::CPU: {
		if (!systemStatus.cpuUsage.empty()) {
			{
				std::lock_guard<std::mutex> lock(systemStatus.cpuMutex);
				mvwprintw(win, (m_rows / 2), 2, "CPU Utilisation: %.2f%%", systemStatus.cpuUsage.front());
			}
		}
		break;
	}

	case Screen::DISK: {
		mvwprintw(win, (m_rows / 2), 2, "Write Speed: %.2f KB/s", systemStatus.writeDisk.load());
		mvwprintw(win, (m_rows / 2) + 1, 2, "Read Speed: %.2f KB/s", systemStatus.readDisk.load());
		if (!systemStatus.diskTime.empty()) {
			{
				std::lock_guard<std::mutex> lock(systemStatus.diskMutex);
				mvwprintw(win, (m_rows / 2) + 2, 2, "Disk Utilisation: %.2f%%", systemStatus.diskTime.front());
			}
		}

		mvwprintw(win, (m_rows / 2) + 4, 2, "Total Space: %.2f GB", systemStatus.totalDisk.load());
		mvwprintw(win, (m_rows / 2) + 5, 2, "Available Space: %.2f GB", systemStatus.availDisk.load());

		wrefresh(win);
		break;
	}

	case Screen::GPU: {
		if (!systemStatus.gpuUsage.empty()) {
			{
				std::lock_guard<std::mutex> lock(systemStatus.gpuMutex);
				mvwprintw(win, (m_rows / 2), 2, "GPU Usage: %.2f%%", systemStatus.gpuUsage.front());
			}
		}

		if (systemStatus.gpuType == GpuType::AMD) {
			mvwprintw(win, (m_rows / 2) + 1, 2, "GPU Temperature: %.1f%°c", systemStatus.gpuTemperature.load());
			mvwprintw(win, (m_rows / 2) + 5, 2, "GPU Clock Speed: %.2f Mhz", systemStatus.gpuClockSpeed.load());
		}
		else if (systemStatus.gpuType == GpuType::NVIDIA) {
			mvwprintw(win, (m_rows / 2) + 1, 2, "GPU Memory Controller: %.2f%%", systemStatus.memControllerUsage.load());
		}

		mvwprintw(win, (m_rows / 2) + 3, 2, "Total VRAM: %.2f GB", systemStatus.vramTotalMemory.load());
		mvwprintw(win, (m_rows / 2) + 4, 2, "Available VRAM: %.2f GB", systemStatus.vramAvailMemory.load());
		break;
	}

	case Screen::MEMORY: {
		if (!systemStatus.memoryUsage.empty()) {
			{
				std::lock_guard<std::mutex> lock(systemStatus.memoryMutex);
				mvwprintw(win, (m_rows / 2), 2, "RAM Usage: %.2f%%", systemStatus.memoryUsage.front());
			}
		}

		mvwprintw(win, (m_rows / 2) + 2, 2, "Total RAM: %.2f GB", systemStatus.ramTotalMemory.load());
		mvwprintw(win, (m_rows / 2) + 3, 2, "Available RAM: %.2f GB", systemStatus.ramAvailMemory.load());
		wrefresh(win);
		break;
	}

	case Screen::NETWORK: {
		if (!systemStatus.networkUsage.empty()) {
			{
				std::lock_guard<std::mutex> lock(systemStatus.networkMutex);
				mvwprintw(win, (m_rows / 2), 2, "Network Usage: %.2f%%", systemStatus.networkUsage.front());
			}
		}

		mvwprintw(win, (m_rows / 2) + 2, 2, "Network Bytes Sent: %.2f Kbps", systemStatus.sendNetwork.load());
		mvwprintw(win, (m_rows / 2) + 3, 2, "Network Bytes Received: %.2f Kbps", systemStatus.receiveNetwork.load());
		break;
	}

	default:
		break;
	}

	wrefresh(win);
}

void GuiController::drawBaseLayout(WINDOW* win, const char* header) {
	wclear(win);

	const char* navigationFooter = "Press left or right arrow keys to traverse the screens";
	const char* exitFooter = "Press enter to exit the application";

	mvwprintw(win, 0, (m_cols - static_cast<int>(strlen(header))) / 2, "%s", header);
	mvwprintw(win, m_rows - 1, (m_cols - static_cast<int>(strlen(navigationFooter))) / 2, "%s", navigationFooter);
	mvwprintw(win, m_rows - 2, (m_cols - static_cast<int>(strlen(exitFooter))) / 2, "%s", exitFooter);

	if (m_currentScreen != Screen::HOME) {
		const int margin = 2;
		int graphHeight = m_rows - 10;
		int graphWidth = m_cols / 2 - margin * 2;
		int startX = m_cols / 2 + margin;
		const int startY = 4;

		m_maxBars = graphWidth - 2;

		drawGraphBox(win, startY, startX, graphHeight, graphWidth, "Utilisation Graph");
	}
	wrefresh(win);
}

void GuiController::drawGraphBox(WINDOW* win, int startY, int startX, int height, int width, const char* title) {

	WINDOW* graphBox = m_screenGraphBoxes[m_currentScreen];

	if (graphBox) {
		delwin(graphBox);
	}

	graphBox = derwin(win, height, width, startY, startX);
	m_screenGraphBoxes[m_currentScreen] = graphBox;

	int colourPair = static_cast<int>(m_currentScreen);

	switch (m_currentScreen) {
	case Screen::CPU: {
		renderGraph(graphBox, systemStatus.cpuUsage, height, colourPair);
		break;
	}
	case Screen::DISK: {
		renderGraph(graphBox, systemStatus.diskTime, height, colourPair);
		break;
	}
	case Screen::GPU: {
		renderGraph(graphBox, systemStatus.gpuUsage, height, colourPair);
		break;
	}
	case Screen::MEMORY: {
		renderGraph(graphBox, systemStatus.memoryUsage, height, colourPair);
		break;
	}
	case Screen::NETWORK: {
		renderGraph(graphBox, systemStatus.networkUsage, height, colourPair);
		break;
	}
	default:
		break;
	}

	box(graphBox, 0, 0);

	mvwprintw(graphBox, 0, (width - static_cast<int>(strlen(title))) / 2, "%s", title);

	mvwprintw(win, startY, startX - 5, "100%%");

	mvwprintw(win, startY + (height / 4), startX - 5, "75%%");

	mvwprintw(win, startY + (height / 2), startX - 5, "50%%");

	mvwprintw(win, (startY + height) - (height / 4), startX - 5, "25%%");

	mvwprintw(win, startY + height - 1, startX - 5, "0%%");

	wrefresh(graphBox);
}

void GuiController::renderGraph(WINDOW* win, const std::deque<double>& data, const int height, const int colourPair) {
	werase(win);

	wattron(win, COLOR_PAIR(colourPair));

	int dataPoints = static_cast<int>(std::min(data.size(), static_cast<size_t>(m_maxBars)));

	for (int i = 1; i < dataPoints; i++) {
		double usage = data[i];
		int barHeight = static_cast<int>(usage / 100.0 * (height - 2));
		for (int j = 0; j < barHeight; j++) {
			mvwaddch(win, height - j - 2, i + 1, ACS_BLOCK);
		}
	}

	wattroff(win, COLOR_PAIR(colourPair));
}

const char* GuiController::getHeader(HeaderType type) const {
	switch (m_currentScreen) {
	case Screen::HOME:
		return  (type == HeaderType::Main) ? "Task Manager Plus" : "";

	case Screen::CPU:
		return (type == HeaderType::Main) ? "CPU Monitor" : "CPU Statistics";

	case Screen::DISK:
		return (type == HeaderType::Main) ? "Disk Monitor" : "Disk Statistics";

	case Screen::GPU:
		return (type == HeaderType::Main) ? "GPU Monitor" : "GPU Statistics";

	case Screen::MEMORY:
		return (type == HeaderType::Main) ? "Memory Monitor" : "Memory Statistics";

	case Screen::NETWORK:
		return (type == HeaderType::Main) ? "Network Monitor" : "Network Statistics";

	default:
		return "Couldn't determine screen correctly! Error.";
	}
}