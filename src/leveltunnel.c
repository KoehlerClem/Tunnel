#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ncurses.h>

#include "leveltunnel.h"
#include "player.h"

void initLevel();
void addNextLine();
void levelLoop();
void printScore();
void randomMode();
void saveScore();
void printStartTunnel();
void addObject();


// Tunnelsettings/daten
static int tunnelEdges[2];
static int tunnelDirection;
static int totalLines;
static char scoreLevelBuffer[31];
static char gameMode;
static int gameSpeed;
char foreground;
char background;


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
	foreground = '#';
	background = ' ';
	tunnelEdges[0] = COLS * 0.4;
	tunnelEdges[1] = COLS * 0.6;
	tunnelDirection = 1;
	totalLines = 0;
	gameSpeed = 33;
	for(int i = 0; i < sizeof(scoreLevelBuffer); i++){
		scoreLevelBuffer[i] = foreground;
	}
	scoreLevelBuffer[31] = '\0';
}


// Zeichnet den geraden Starttunnel
void printStartTunnel(){
	for(int i = 0; i < LINES; i++){
		for(int x = 0; x < COLS; x++){
			if( (tunnelEdges[0] < x) && (tunnelEdges[1] > x) ){
				mvaddch(i, x, background);
			}else{
				mvaddch(i, x, foreground);
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

		addObject();
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
			mvaddch(0, i, background);
		}else {
			mvaddch(0, i, foreground);
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
	switch ( rand()%10 ) {
		case 0: tunnelDirection = 1; break;
		case 1: tunnelDirection = -1; break;
		default: break;
	}
}

void addObject(){
	char object = 0;
	switch (rand()%100) {
		case 0: object = '$'; break;
		case 1: object = foreground; break;
		default: break;
	}
	if(object){
		int *edges = getLineEdges(0);
		int objCol = edges[0] + ( rand() % (edges[1]-edges[0]) ) + 1;
		mvaddch(0, objCol, object);
	}

}

/////////////////////// END LEVEL LOOP METHODEN////////////////

int getTotalLines(){
	return totalLines;
}

int * getLineEdges(int line){
	static int edges[2];

	for(int i = 0; i < COLS; i++){
		if( (mvinch(line, i) & A_CHARTEXT) == background){
			edges[0] = (i-1);
			for(int a = i; a < COLS; a++){
				if( ( (mvinch(line, a) & A_CHARTEXT) == foreground) || (a == (COLS-1))){
					edges[1] = a;
					break;
				}
			}
			break;
		}
	}
	return edges;
}