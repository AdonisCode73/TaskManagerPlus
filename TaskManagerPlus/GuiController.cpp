#include "GuiController.h"

void GuiController::guiInit() {
	initscr();             
	noecho();               
	curs_set(FALSE);        
	keypad(stdscr, TRUE);  

	getmaxyx(stdscr, rows, cols);

	homeW = newwin(rows, cols, 0, 0);
	cpuW = newwin(rows, cols, 0, 0);
	diskW = newwin(rows, cols, 0, 0);
	gpuW = newwin(rows, cols, 0, 0);
	memoryW = newwin(rows, cols, 0, 0);
	networkW = newwin(rows, cols, 0, 0);

	currentScreen = HOME;
	screenIdx = 0;

	firstRun = { {cpuW, true}, {diskW, true}, {gpuW, true}, {memoryW, true}, {networkW, true} };

	clear();
}

void GuiController::start() {
	guiInit();

	isRunning = true;
	guiThread = std::thread(&GuiController::updatePage, this);

	navigateWindows();
	
	stop();
}

void GuiController::navigateWindows() {
	changeWindow();
	while ((userInput = getch()) != '\n') {
		switch (userInput) {
		case KEY_LEFT:
			screenIdx = (screenIdx == 0) ? NUM_WINDOWS - 1 : screenIdx - 1;
			currentScreen = static_cast<Screen>(screenIdx);
			changeWindow();
			break;
		case KEY_RIGHT:
			screenIdx = (screenIdx == NUM_WINDOWS - 1) ? 0 : screenIdx + 1;
			currentScreen = static_cast<Screen>(screenIdx);
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
	switch (currentScreen) {
	case HOME: {
		drawBaseLayout(homeW, "Task Manager Plus", "Press left or right to traverse the screens");
		break;
	}
	case CPU: {
		drawBaseLayout(cpuW, "CPU Monitor", "Press left or right to traverse the screens");
		drawCPUPage(cpuW);
		break;
	}
	case DISK: {
		drawBaseLayout(diskW, "Disk Monitor", "Press left or right to traverse the screens");
		drawDiskPage(diskW);
		break;
	}
	case GPU: {
		drawBaseLayout(gpuW, "GPU Monitor", "Press left or right to traverse the screens");
		drawGPUPage(gpuW);
		break;
	}
	case MEMORY: {
		drawBaseLayout(memoryW, "Memory Monitor", "Press left or right to traverse the screens");
		drawMemoryPage(memoryW);
		break;
	}
	case NETWORK: {
		drawBaseLayout(networkW, "Network Monitor", "Press left or right to traverse the screens");
		drawNetworkPage(networkW);
		break;
	}
	default:
		break;
	}
}

void GuiController::updatePage() {
	while (isRunning) {
		changeWindow();
		refresh();
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

void GuiController::drawBaseLayout(WINDOW* win, const char* title, const char* footer) {
	wclear(win);

	mvwprintw(win, 0, (cols - strlen(title)) / 2, "%s", title);
	mvwprintw(win, rows - 1, (cols - strlen(footer)) / 2, "%s", footer);

	drawGraphBox(win, 5, (cols / 2) - 22, 20, 75, "Utilisation Graph");
	wrefresh(win);
}

void GuiController::drawCPUPage(WINDOW* win) {
	const char* header = "CPU Utilisation";

	mvwprintw(win, (rows / 2) - 2, 2, "%s", header);
	mvwprintw(win, (rows / 2), 2, "CPU Utilisation: %.2f%%", systemStatus.cpuUsage.load());
	wrefresh(win);
}

void GuiController::drawDiskPage(WINDOW* win) {
	const char* header = "Disk Utilisation";

	mvwprintw(win, (rows / 2) - 2, 2, "%s", header);

	mvwprintw(win, (rows / 2), 2, "Write Speed: %.2f KB/s", systemStatus.writeDisk.load());
	mvwprintw(win, (rows / 2) + 1, 2, "Read Speed: %.2f KB/s", systemStatus.readDisk.load());
	mvwprintw(win, (rows / 2) + 2, 2, "Disk Utilisation: %.2f%%", systemStatus.diskTime.load());

	mvwprintw(win, (rows / 2) + 4, 2, "Total Space: %.2f GB", systemStatus.totalDisk.load());
	mvwprintw(win, (rows / 2) + 5, 2, "Available Space: %.2f GB", systemStatus.availDisk.load());

	wrefresh(win);
}

void GuiController::drawGPUPage(WINDOW* win) {
	const char* header = "GPU Utilisation";

	mvwprintw(win, (rows / 2) - 2, 2, "%s", header);

	mvwprintw(win, (rows / 2), 2, "GPU Usage: %.2f%%", systemStatus.gpuUsage.load());
	mvwprintw(win, (rows / 2) + 1, 2, "Memory Controller Usage: %.2f%%", systemStatus.memControllerUsage.load());

	mvwprintw(win, (rows / 2) + 3, 2, "Total VRAM: %.2f GB", systemStatus.vramTotalMemory.load());
	mvwprintw(win, (rows / 2) + 4, 2, "Available VRAM: %.2f GB", systemStatus.vramAvailMemory.load());

	wrefresh(win);
}

void GuiController::drawMemoryPage(WINDOW* win) {
	const char* header = "RAM Utilisation";

	mvwprintw(win, (rows / 2) - 2, 2, "%s", header);

	mvwprintw(win, (rows / 2), 2, "RAM Usage: %.2f%%", systemStatus.memoryUsage.load());

	mvwprintw(win, (rows / 2) + 2, 2, "Total RAM: %.2f GB", systemStatus.ramTotalMemory.load());
	mvwprintw(win, (rows / 2) + 3, 2, "Available RAM: %.2f GB", systemStatus.ramAvailMemory.load());
	wrefresh(win);
}

void GuiController::drawNetworkPage(WINDOW* win) {
	const char* header = "Network Utilisation";

	mvwprintw(win, (rows / 2) - 2, 2, "%s", header);

	mvwprintw(win, (rows / 2), 2, "Network Bytes Sent: %.2f Kbps", systemStatus.sendNetwork.load());
	mvwprintw(win, (rows / 2) + 1, 2, "Network Bytes Received: %.2f Kbps", systemStatus.receiveNetwork.load());

	wrefresh(win);
}

void GuiController::drawGraphBox(WINDOW* win, int startY, int startX, int height, int width, const char* title) {

	WINDOW* graphBox = windowGraphBox[win];
	
	if (!graphBox) {
		graphBox = derwin(win, height, width, startY, startX);
		windowGraphBox[win] = graphBox;
	}

	box(graphBox, 0, 0);

	mvwprintw(graphBox, 0, (width - strlen(title)) / 2, "%s", title);

	wrefresh(graphBox);
}

void GuiController::renderCPUGraph(WINDOW* win) {

}

void GuiController::stop() {
	isRunning = false;
	if (guiThread.joinable()) {
		guiThread.join();
	}
	endwin();
}