#include <stdlib.h>
#include <ncurses.h>

#include "game.h"
#include "leveltunnel.h"
#include "snake.h"


char printStartMenu();

// Startet das Tunnel Spiel
void startGame(){

	//Gamemode zzzzzz
	int gameModeChoice;

	// Levelscreen wird initializiert
	initscr();
	noecho();
	curs_set(0);
	cbreak();

	// Menu Loop
	while((gameModeChoice = printStartMenu()) != 'q'){
		switch(gameModeChoice){

			case 'c': runLevel(gameModeChoice); break;
			case 'r': runLevel(gameModeChoice); break;
			case 's': runSnake(); break;
			default: break;
			clear();
			refresh();
		}
	}
	endwin();
	return;
}

// Zeichnet das Startmenu und gibt den Spielerinput zurück
char printStartMenu(){
	clear();
	move(0,0);
	int stringLength;
	char tunnelChillMode[] = "Tunnel -chillmode (c)";
	stringLength = sizeof(tunnelChillMode);
	mvprintw( (LINES/2), ((COLS/2)-(stringLength/2)), tunnelChillMode);

	char tunnelRandom[] = "Tunnel -random (r)";
	stringLength = sizeof(tunnelRandom);
	mvprintw( (LINES/2) + 1, ((COLS/2)-(stringLength/2)), tunnelRandom);

	char snake[] = "SNAKE (s)";
	stringLength = sizeof(snake);
	mvprintw( (LINES/2) + 2, ((COLS/2)-(stringLength/2)), snake);


	char quit[] = "quit (q)";
	stringLength = sizeof(quit);
	mvprintw( ( (LINES/2) + 3), ((COLS/2)-(stringLength/2)), quit);
	refresh();
	return getch();
}
