#include <stdlib.h>
#include <stdio.h>
#include "tunnel.h"
#include <ncurses.h>
#include "player.h"
#include <time.h>

void initLevel();
void addNextLine();
void levelLoop();
void printScore();
void randomMode();
void saveScore();
void printStartTunnel();


// Tunnelsettings/daten
static int tunnelEdges[2];
static int tunnelDirection;
static int totalLines;
static char scoreLevelBuffer[31];
static char gameMode;
int gameSpeed;


// Hier läuft das ganze Spiel ab
void runLevel(char gameModeChoice){
	gameMode = gameModeChoice;
	initLevel();
	initPlayer();
	printStartTunnel();
	levelLoop();
}

// Bereitet die Variablen vor
void initLevel(){

	// Terminal vorbereiten
	clear();
	refresh();
	nodelay(stdscr, TRUE);
	srand(time(NULL));

	//Tunneldaten setzten
	tunnelEdges[0] = COLS * 0.4;
	tunnelEdges[1] = COLS * 0.6;
	tunnelDirection = 1;
	totalLines = 0;
	gameSpeed = 33;
	for(int i = 0; i < sizeof(scoreLevelBuffer); i++){
		scoreLevelBuffer[i] = '#';
	}
	scoreLevelBuffer[31] = '\0';
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

	char playerInput;

	while( ( (playerInput = getch()) != 'm') && (getLifes() > -1)){

		if(playerInput == 'p'){
			nodelay(stdscr, FALSE);
			playerInput = getch();
			nodelay(stdscr, TRUE);
		}
		if(gameMode == 'r') {
			randomMode();
		}
		addNextLine();
		printScore();
		printPlayerAction(playerInput);
		refresh();
		napms(gameSpeed);
		if(getLifes() < 0){
			saveScore();
		}
	}
	nodelay(stdscr, FALSE);
}

// Fügt eine neue Tunnelzeile hinzu
void addNextLine(){

	move(LINES-1, 0);
	deleteln();
	move(0,0);
	insertln();

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
	}else if( tunnelEdges[1] > (COLS - 3) ){
		tunnelDirection = -1;
	}
}

// Zeichnet bei jedem aufruf den Score und speichert die überschriebene Zeile
void printScore() {
	mvprintw(1, (COLS-31), scoreLevelBuffer);
	mvinnstr(0, (COLS-31), scoreLevelBuffer, 30);
	mvprintw(0, (COLS-31), "  Score: %09i Lifes: %02i  ", totalLines, getLifes());
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
	int i = 0;
	switch ( (i = rand())%10 ) {
		case 0: tunnelDirection = 1; break;
		case 1: tunnelDirection = -1; break;
		default: break;
	}
}

/////////////////////// END LEVEL LOOP METHODEN////////////////

int getTotalLines(){
	return totalLines;
}
