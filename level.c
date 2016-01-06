#include <stdlib.h>
#include <stdio.h>
#include "tunnel.h"
#include <ncurses.h>

void initLevel();
void addNextLine();
void levelLoop();
void printScore();
void randomMode();
void saveScore();


// Tunnelsettings/daten
static int tunnelEdges[2];
static int tunnelDirection;
static int totalLines;
static char scoreLevelBuffer[31];
static char gameMode;

// Hier läuft das ganze Spiel ab
void runLevel(char gameModeChoice){
	gameMode = gameModeChoice;
	initLevel();
	printStartTunnel();
	levelLoop();
}

// Bereitet die Variablen vor
void initLevel(){

	// Terminal vorbereiten
	clear();
	refresh();
	nodelay(stdscr, TRUE);

	//Tunneldaten setzten
	int tunnelEdgeLeft = COLS  0.4;
	int tunnelEdgeRight = COLS  0.6;
	int tunnelDirection = 1;
	int totalLines = 0;
	char scoreLevelBuffer[31] = {
		"###############################"
	};
}


// Zeichnet den geraden Starttunnel
void printStartTunnel(){
	for(int i = 0; i < LINES; i++){
		for(int x = 0; x < COLS; x++){
			if( (tunnelEdges[0] < x) && (tunnelEdges[1] > x) ){
				mvaddch(i, x, ' ');
			}else{
				mvaddch(i, x, '#');
			}
		}
	}
	refresh();
}

/////////////////////// LEVEL LOOP METHODEN////////////////////
// Zeichnet pro Durchlauf eine neue Zeile und
// wertet den möglichen Spielerinput aus
void levelLoop(){

	while( ( (playerInput = getch()) != 'm') && (lifes > -1)){

		if(playerInput == 'p'){
			nodelay(stdscr, FALSE);
			playerInput = getch();
			nodelay(stdscr, TRUE);
		}

		addNextLine();
		if(gameMode == 'r') {
			randomMode();
		}
		printScore();
		printPlayerAction(playerInput);
		refresh();
		napms(gameSpeed);
	}
	nodelay(stdscr, FALSE);
}

// Fügt eine neue Tunnelzeile hinzu
void addNextLine(){

	move(LINES-1, 0);
	deleteln();
	move(0,0);
	insertln();
	int colBuffer = ( (totalLines) + (LINES-5)) % (LINES-4);
	int colBuffer2 = ( (totalLines) + (LINES-6)) % (LINES-4);

	(totalLines)++;

	tunnelEdges[0] += tunnelDirection;
	tunnelEdges[1] += tunnelDirection;

	for(int i = 0; i < COLS; i++){
		if( (i > tunnelEdges[0]) && (i < tunnelEdges[1])){
			mvaddch(0, i, ' ');
		}else {
			mvaddch(0, i, '#');
		}
	}
	if(tunnelEdges[0] < 2){
		tunnelDirection = 1;
	}else{
		tunnelDirection = -1;
	}
}

// Zeichnet bei jedem aufruf den Score und speichert die überschriebene Zeile
void printScore(int lines, char scoreLevelBuffer[31], int lifes) {
	mvprintw(1, (COLS-33), scoreLevelBuffer);
	mvinstr(0, (COLS-33), scoreLevelBuffer);
	mvprintw(0, (COLS-33), "  Score: %09i Lifes: %02i  ", lines, lifes);
}

// Speichert den Score in einer txt
void saveScore(){
	FILE *f = fopen("highscore.txt", "a");
	if (f == NULL)
	{
		printf("Error opening file!\n");
		exit(1);
	}
	int i = 1;
	fprintf(f, "Player: %-20s Hightscore: %09i\n", "Testplayer",totalLines);
	fclose(f);
}

void randomMode(){
	switch (rand()% 35) {
		case 0: tunnelDirection = 1; break;
		case 1: tunnelDirection = -1; break;
		default: break;
	}
}

/////////////////////// END LEVEL LOOP METHODEN////////////////

int getTotalLines(){
	return totalLines;
}
