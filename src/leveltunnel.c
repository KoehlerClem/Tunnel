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
void printClearLevelFromBuffer();
void initLevelBuffer(char ***buffer);
void freeBuffer(char ***buffer);


// Tunnelsettings/daten
static int tunnelEdges[2];
static int tunnelDirection[2];
static int totalLines;
static char gameMode;
static int gameSpeed;
static char **levelBuffer;
static char **oldLevelBuffer;
static int bufferDimensions[2];

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
	foreground = '.';
	background = ' ';
	tunnelEdges[0] = COLS * 0.4;
	tunnelEdges[1] = COLS * 0.6;
	tunnelDirection[0] = 1;
	tunnelDirection[1] = 1;
	totalLines = 0;
	gameSpeed = 33;
	bufferDimensions[0] = 3;
	bufferDimensions[1] = COLS;
	initLevelBuffer(&levelBuffer);
	initLevelBuffer(&oldLevelBuffer);
}


// Zeichnet den geraden Starttunnel
void printStartTunnel(){
	for(int i = 0; i < LINES; i++){
		for(int x = 0; x < COLS; x++){
			if( (tunnelEdges[0] < x) && (tunnelEdges[1] > x) ){
				mvaddch(i, x, background);
				if(i < bufferDimensions[0]){
					levelBuffer[i][x] = background;
				}
			}else{
				mvaddch(i, x, foreground);
				if(i < bufferDimensions[0]){
					levelBuffer[i][x] = foreground;
				}
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

		if( (totalLines%100) == 0){
			gameSpeed--;
		}

		addNextLine();
		if(gameMode == 'r') {
			randomMode();
		}
		//addObject();
		printClearLevelFromBuffer();
		printScore();
		printPlayerAction(playerInput);
		printScore();
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
	tunnelEdges[0] += tunnelDirection[0];
	tunnelEdges[1] += tunnelDirection[1];
	for(int i = 0; i < COLS; i++){
		if( (i > tunnelEdges[0]) && (i < tunnelEdges[1])){
			mvaddch(0, i, background);
		}else {
			mvaddch(0, i, foreground);
		}
	}
	if(tunnelEdges[0] < 2){
		tunnelDirection[0] = 1;
		tunnelDirection[1] = 1;
	}else if( tunnelEdges[1] > (COLS - 3)){
		tunnelDirection[0] = -1;
		tunnelDirection[1] = -1;
	}
}

// Zeichnet die überschriebenen Lines wieder nach
void printClearLevelFromBuffer(){

	// wiedeherstellung der 3 Zeile ( [2] )!
	mvprintw(bufferDimensions[0], 0, "%s", levelBuffer[bufferDimensions[0]-1]);

	// Kopiert das alte level in einen Zwischenspeicher
	initLevelBuffer(&oldLevelBuffer);
	for(int i = 0; i < bufferDimensions[0]; i++){
		for(int j = 0; j < bufferDimensions[1]; j++){
			oldLevelBuffer[i][j] = levelBuffer[i][j];
		}
	}
	freeBuffer(&levelBuffer);
	initLevelBuffer(&levelBuffer);
	// schreibt die neue levelZeile in den Buffer
	for(int i = 0; i < bufferDimensions[1]; i++){
		levelBuffer[0][i] = (mvinch(0, i) & A_CHARTEXT);
	}
	// schreibt die alten levelZeilen aus dem altem oldLevelBuffer in den neuen
	for(int i = 1; i < bufferDimensions[0]; i++){
		for(int j = 0; j < bufferDimensions[1]; j++){
			levelBuffer[i][j] = oldLevelBuffer[i-1][j];
		}
	}
	freeBuffer(&oldLevelBuffer);
}

// Zeichnet bei jedem aufruf den Score und speichert die überschriebene Zeile
void printScore() {
	mvprintw(0, (COLS-20), "  Score: %9i  ", totalLines);
	mvprintw(1, (COLS-20), "  Lifes: %9i  ", getLifes());
	mvprintw(2, (COLS-20), "  Shield: %8i  ", getShieldHP());
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
	switch ( rand()%30 ) {
		case 0: tunnelDirection[0] *= -1; tunnelDirection[1] *= -1; break;
		case 1: tunnelDirection[0] *= -1; tunnelDirection[1] *= -1;break;
		default: break;
	}
}

void addObject(){
	char baddie = '-';
	char cookie = '+';
	int *edges = getLineEdges(0);
	int middle = edges[0] + (edges[1]-edges[0])/2;
	mvaddch(0, edges[0]+1, baddie);
	mvaddch(0, middle, cookie);
	mvaddch(0, edges[1]-1, baddie);
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

// Param: Ein Pointer auf ein char-Array was nach
// einem Koordinatensystem geordnet ist (2dim., lines, cols)
void initLevelBuffer(char ***buffer){

	*buffer = calloc(bufferDimensions[0], sizeof(char*));
	for(int i = 0; i < bufferDimensions[0]; i++){
		(*buffer)[i] = calloc(bufferDimensions[1], sizeof(char));
	}
}

void freeBuffer(char ***buffer){
	for(int i = 0; i < bufferDimensions[0]; i++){
		free((*buffer)[i]);
	}
	free(*buffer);
}

int getGameSpeed(){
	return gameSpeed;
}

void setGameSpeed(int newGameSpeed){
	gameSpeed = newGameSpeed;
}
