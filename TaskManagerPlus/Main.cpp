#include <iostream>
#include <curses.h>
#include "CpuMonitor.h"
#include "MemoryMonitor.h"
#include "MonitorUtils.h"

int main()
{
	MonitorUtils utilMonitor;

	std::cout << "\n**Beginning System Monitoring**\n";

	std::cout << "****Press Enter to exit****\n";
	utilMonitor.start();
	std::cin.get(); // TODO: Come up with a better solution to input not being recognised

	utilMonitor.stop();
	std::cout << "Safely Exiting application.";

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
