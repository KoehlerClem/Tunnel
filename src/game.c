#include <stdlib.h>
#include <ncurses.h>

#include "game.h"
#include "leveltunnel.h"


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
			default:runLevel(gameModeChoice);
		}
	}
	endwin();
	return;
}

// Zeichnet das Startmenu und gibt den Spielerinput zurück
char printStartMenu(){
	clear();
	move(0,0);
	char gameName[] = "Tunnel -chillmode (anykey)";
	int gameNameLength = sizeof(gameName);
	mvprintw( (LINES/2), ((COLS/2)-(gameNameLength/2)), gameName);

	char gameName1[] = "Tunnel -random (r)";
	int gameNameLength1 = sizeof(gameName1);
	mvprintw( (LINES/2) + 1, ((COLS/2)-(gameNameLength1/2)), gameName1);


	char quit[] = "quit (q)";
	int quitLength = sizeof(quit);
	mvprintw( ( (LINES/2) + 2), ((COLS/2)-(quitLength/2)), quit);
	refresh();
	return getch();
}
