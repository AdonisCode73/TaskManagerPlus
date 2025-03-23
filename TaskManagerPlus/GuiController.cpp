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
	int ch;
	while ((ch = getch()) != '\n') {
		switch (ch) {
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
		clear();
		wrefresh(homeW);
		const char* title = "Task Manager Plus";
		mvprintw(0, (cols - strlen(title)) / 2, "%s", title);

		const char* middle = "Welcome to my application";
		mvprintw(rows / 2, (cols - strlen(middle)) / 2, "%s", middle);

		const char* footer = "Press left or right to traverse the screens";
		mvprintw(rows - 1, (cols - strlen(footer)) / 2, "%s", footer);
		refresh();
		break;
	}
	case CPU:
		clear();
		wrefresh(cpuW);
		break;
	case DISK:
		clear();
		wrefresh(diskW);
		break;
	case GPU:
		clear();
		wrefresh(gpuW);
		break;
	case MEMORY:
		clear();
		wrefresh(memoryW);
		break;
	case NETWORK:
		clear();
		wrefresh(networkW);
		break;
	default:
		break;
	}
}

void GuiController::stop() {
	endwin();
}