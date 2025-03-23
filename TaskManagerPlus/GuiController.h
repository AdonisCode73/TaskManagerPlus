#pragma once
#include <curses.h>
#include <iostream>

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

		int rows, cols;
		int screenIdx;

		WINDOW* homeW;
		WINDOW* cpuW;
		WINDOW* diskW;
		WINDOW* gpuW;
		WINDOW* memoryW;
		WINDOW* networkW;

		Screen currentScreen;
};