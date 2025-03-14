#include <iostream>
#include <curses.h>
#include "CpuMonitor.h"
#include "MemoryMonitor.h"
#include "MonitorUtils.h"
#include <CL/cl.h>

int main()
{
	MonitorUtils utilMonitor;
	static std::string exitKey;

	std::cout << "\n**Beginning System Monitoring**\n";

	std::cout << "****Press Enter to exit****\n";
	utilMonitor.start();
	
	while (utilMonitor.isRunningCheck()) {
	}

	utilMonitor.stop();
	std::cout << "\nSafely Exiting application.";

	/*
	initscr();            // Initialize ncurses mode
	printw("Hello, ncurses!"); // Print text
	refresh();            // Update the screen
	getch();              // Wait for user input
	printw("I am best C++ Programmer!");
	getch();
	clear();
	endwin();*/
}
