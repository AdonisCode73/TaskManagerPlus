#include <iostream>
#include <curses.h>
#include "CpuMonitor.h"
#include "MemoryMonitor.h"

int main()
{
	CpuMonitor cpuMonitor;
	MemoryMonitor memoryMonitor;

	std::cout << "\n**Beginning System Monitoring**\n";

	memoryMonitor.start();
	cpuMonitor.start();

	std::cout << "****Press Enter to exit****\n";
	std::cin.get();

	cpuMonitor.stop();
	memoryMonitor.stop();
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
