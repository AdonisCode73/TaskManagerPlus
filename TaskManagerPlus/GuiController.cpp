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

	clear();
}

void GuiController::start() {
	guiInit();

	navigateWindows();

	stop();
}

void GuiController::navigateWindows() {
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

void GuiController::drawBaseLayout(WINDOW* win, const char* title, const char* footer) {
	clear();
	wrefresh(win);

	mvprintw(0, (cols - strlen(title)) / 2, "%s", title);
	mvprintw(rows - 1, (cols - strlen(footer)) / 2, "%s", footer);

	refresh();
}

void GuiController::drawCPUPage(WINDOW* win) {
	//const char* cpuStat = "CPU Utilisation: %.2f%%";
	//mvprintw(5, (cols - strlen(cpuStat)) / 2, "CPU Utilisation: %.2f%%", systemStatus.cpuUsage);
}

void GuiController::drawDiskPage(WINDOW* win) {

}

void GuiController::drawGPUPage(WINDOW* win) {

}

void GuiController::drawMemoryPage(WINDOW* win) {

}

void GuiController::drawNetworkPage(WINDOW* win) {

}

void GuiController::stop() {
	endwin();
}