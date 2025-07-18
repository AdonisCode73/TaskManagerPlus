#include "GuiController.h"

void GuiController::guiInit() {
	initscr();
	noecho();
	curs_set(FALSE);
	keypad(stdscr, TRUE);

	getmaxyx(stdscr, m_rows, m_cols);

	m_homeW = newwin(m_rows, m_cols, 0, 0);
	m_cpuW = newwin(m_rows, m_cols, 0, 0);
	m_diskW = newwin(m_rows, m_cols, 0, 0);
	m_gpuW = newwin(m_rows, m_cols, 0, 0);
	m_memoryW = newwin(m_rows, m_cols, 0, 0);
	m_networkW = newwin(m_rows, m_cols, 0, 0);

	m_currentScreen = HOME;
	m_screenIdx = 0;

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

	m_isRunning = true;
	m_guiThread = std::thread(&GuiController::updatePage, this);

	navigateWindows();

	stop();
}

void GuiController::navigateWindows() {
	changeWindow();
	int userInput;
	while ((userInput = getch()) != '\n') {
		switch (userInput) {
		case KEY_LEFT:
			m_screenIdx = (m_screenIdx == 0) ? NUM_WINDOWS - 1 : m_screenIdx - 1;
			m_currentScreen = static_cast<Screen>(m_screenIdx);
			changeWindow();
			break;
		case KEY_RIGHT:
			m_screenIdx = (m_screenIdx == NUM_WINDOWS - 1) ? 0 : m_screenIdx + 1;
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
	switch (m_currentScreen) {
	case HOME: {
		drawBaseLayout(m_homeW, "Task Manager Plus", "Press left or right arrow keys to traverse the screens");
		drawHomePage(m_homeW);
		break;
	}
	case CPU: {
		drawBaseLayout(m_cpuW, "CPU Monitor", "Press left or right arrow keys to traverse the screens");
		drawCPUPage(m_cpuW);
		break;
	}
	case DISK: {
		drawBaseLayout(m_diskW, "Disk Monitor", "Press left or right arrow keys to traverse the screens");
		drawDiskPage(m_diskW);
		break;
	}
	case GPU: {
		drawBaseLayout(m_gpuW, "GPU Monitor", "Press left or right arrow keys to traverse the screens");
		drawGPUPage(m_gpuW);
		break;
	}
	case MEMORY: {
		drawBaseLayout(m_memoryW, "Memory Monitor", "Press left or right arrow keys to traverse the screens");
		drawMemoryPage(m_memoryW);
		break;
	}
	case NETWORK: {
		drawBaseLayout(m_networkW, "Network Monitor", "Press left or right arrow keys to traverse the screens");
		drawNetworkPage(m_networkW);
		break;
	}
	default:
		break;
	}
}

void GuiController::updatePage() {
	while (m_isRunning) {
		changeWindow();
		refresh();
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

void GuiController::drawBaseLayout(WINDOW* win, const char* title, const char* footer) {
	wclear(win);

	mvwprintw(win, 0, (m_cols - static_cast<int>(strlen(title))) / 2, "%s", title);
	mvwprintw(win, m_rows - 1, (m_cols - static_cast<int>(strlen(footer))) / 2, "%s", footer);

	if (m_currentScreen != HOME) {
		drawGraphBox(win, 5, (m_cols / 2) - 22, 20, 75, "Utilisation Graph");
	}
	wrefresh(win);
}

void GuiController::drawHomePage(WINDOW* win) {
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

	wrefresh(win);
}

void GuiController::drawCPUPage(WINDOW* win) {
	const char* header = "CPU Utilisation";

	mvwprintw(win, (m_rows / 2) - 2, 2, "%s", header);
	if (!systemStatus.cpuUsage.empty()) {
		{
			std::lock_guard<std::mutex> lock(systemStatus.cpuMutex);
			mvwprintw(win, (m_rows / 2), 2, "CPU Utilisation: %.2f%%", systemStatus.cpuUsage.front());
		}
	}
	wrefresh(win);
}

void GuiController::drawDiskPage(WINDOW* win) {
	const char* header = "Disk Utilisation";

	mvwprintw(win, (m_rows / 2) - 2, 2, "%s", header);

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
}

void GuiController::drawGPUPage(WINDOW* win) {
	const char* header = "GPU Utilisation";

	mvwprintw(win, (m_rows / 2) - 2, 2, "%s", header);

	if (!systemStatus.gpuUsage.empty()) {
		{
			std::lock_guard<std::mutex> lock(systemStatus.gpuMutex);
			mvwprintw(win, (m_rows / 2), 2, "GPU Usage: %.2f%%", systemStatus.gpuUsage.front());
		}
	}

#ifdef USE_AMD
	mvwprintw(win, (m_rows / 2) + 1, 2, "GPU Temperature: %.1f%°c", systemStatus.gpuTemperature.load());
	mvwprintw(win, (m_rows / 2) + 5, 2, "GPU Clock Speed: %.2f Mhz", systemStatus.gpuClockSpeed.load());
#endif

#ifdef USE_NVIDIA
	mvwprintw(win, (m_rows / 2) + 1, 2, "GPU Temperature: %.2f%%", systemStatus.memControllerUsage.load());
#endif
	mvwprintw(win, (m_rows / 2) + 3, 2, "Total VRAM: %.2f GB", systemStatus.vramTotalMemory.load());
	mvwprintw(win, (m_rows / 2) + 4, 2, "Available VRAM: %.2f GB", systemStatus.vramAvailMemory.load());

	wrefresh(win);
}

void GuiController::drawMemoryPage(WINDOW* win) {
	const char* header = "RAM Utilisation";

	mvwprintw(win, (m_rows / 2) - 2, 2, "%s", header);

	if (!systemStatus.memoryUsage.empty()) {
		{
			std::lock_guard<std::mutex> lock(systemStatus.memoryMutex);
			mvwprintw(win, (m_rows / 2), 2, "RAM Usage: %.2f%%", systemStatus.memoryUsage.front());
		}
	}

	mvwprintw(win, (m_rows / 2) + 2, 2, "Total RAM: %.2f GB", systemStatus.ramTotalMemory.load());
	mvwprintw(win, (m_rows / 2) + 3, 2, "Available RAM: %.2f GB", systemStatus.ramAvailMemory.load());
	wrefresh(win);
}

void GuiController::drawNetworkPage(WINDOW* win) {
	const char* header = "Network Utilisation";

	mvwprintw(win, (m_rows / 2) - 2, 2, "%s", header);

	if (!systemStatus.networkUsage.empty()) {
		{
			std::lock_guard<std::mutex> lock(systemStatus.networkMutex);
			mvwprintw(win, (m_rows / 2), 2, "Network Usage: %.2f%%", systemStatus.networkUsage.front());
		}
	}

	mvwprintw(win, (m_rows / 2) + 2, 2, "Network Bytes Sent: %.2f Kbps", systemStatus.sendNetwork.load());
	mvwprintw(win, (m_rows / 2) + 3, 2, "Network Bytes Received: %.2f Kbps", systemStatus.receiveNetwork.load());

	wrefresh(win);
}

void GuiController::drawGraphBox(WINDOW* win, int startY, int startX, int height, int width, const char* title) {

	WINDOW* graphBox = windowGraphBox[win];

	if (!graphBox) {
		graphBox = derwin(win, height, width, startY, startX);
		windowGraphBox[win] = graphBox;
	}

	switch (m_currentScreen) {
	case CPU: {
		renderCPUGraph(windowGraphBox[win], height, width);
		break;
	}
	case DISK: {
		renderDiskGraph(windowGraphBox[win], height, width);
		break;
	}
	case GPU: {
		renderGPUGraph(windowGraphBox[win], height, width);
		break;
	}
	case MEMORY: {
		renderMemoryGraph(windowGraphBox[win], height, width);
		break;
	}
	case NETWORK: {
		renderNetworkGraph(windowGraphBox[win], height, width);
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

void GuiController::renderCPUGraph(WINDOW* win, int height, int width) {
	werase(win);

	wattron(win, COLOR_PAIR(1));

	for (int i = 1; i < systemStatus.cpuUsage.size(); i++) {
		double currCpuUtil = std::ceil(systemStatus.cpuUsage[i - 1] / 5.0);
		for (int j = 0; j < currCpuUtil; j++) {
			mvwaddch(win, height - j - 1, i, ACS_BLOCK);
		}
	}

	wattroff(win, COLOR_PAIR(1));
}

void GuiController::renderDiskGraph(WINDOW* win, int height, int width) {
	werase(win);

	wattron(win, COLOR_PAIR(2));

	for (int i = 1; i < systemStatus.diskTime.size(); i++) {
		double currDiskUtil = std::ceil(systemStatus.diskTime[i - 1] / 5.0);
		for (int j = 0; j < currDiskUtil; j++) {
			mvwaddch(win, height - j - 1, i, ACS_BLOCK);
		}
	}

	wattroff(win, COLOR_PAIR(2));
}

void GuiController::renderGPUGraph(WINDOW* win, int height, int width) {
	werase(win);

	wattron(win, COLOR_PAIR(3));

	for (int i = 1; i < systemStatus.gpuUsage.size(); i++) {
		double currGpuUtil = std::ceil(systemStatus.gpuUsage[i - 1] / 5.0);
		for (int j = 0; j < currGpuUtil; j++) {
			mvwaddch(win, height - j - 1, i, ACS_BLOCK);
		}
	}

	wattroff(win, COLOR_PAIR(3));
}

void GuiController::renderMemoryGraph(WINDOW* win, int height, int width) {
	werase(win);

	wattron(win, COLOR_PAIR(4));

	for (int i = 1; i < systemStatus.memoryUsage.size(); i++) {
		double currMemoryUtil = std::ceil(systemStatus.memoryUsage[i - 1] / 5.0);
		for (int j = 0; j < currMemoryUtil; j++) {
			mvwaddch(win, height - j - 1, i, ACS_BLOCK);
		}
	}

	wattroff(win, COLOR_PAIR(4));
}

void GuiController::renderNetworkGraph(WINDOW* win, int height, int width) {
	werase(win);

	wattron(win, COLOR_PAIR(5));

	for (int i = 1; i < systemStatus.networkUsage.size(); i++) {
		double currNetworkUtil = std::ceil(systemStatus.networkUsage[i - 1] / 5.0);
		for (int j = 0; j < currNetworkUtil; j++) {
			mvwaddch(win, height - j - 1, i, ACS_BLOCK);
		}
	}

	wattroff(win, COLOR_PAIR(5));
}

void GuiController::stop() {
	m_isRunning = false;
	if (m_guiThread.joinable()) {
		m_guiThread.join();
	}
	endwin();
}