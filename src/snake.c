#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#include "game.h"
#include "snake.h"


void initSnake();
void snakeLevelLoop();
void moveHead();
void deleteTail();
void setCake();
void checkNextField();
void exitSnake();

// Deklaration der Variablen
static char player;
static char cake;
static char playerInput;
static char lastInput;
static char bg;
static int lifes;
static int length;
static int headPos[2];
static int cakeSet;

static int addOne;
static int maxLength;
static int counter;
static int gameSpeed;

typedef struct snakeElem{
	int coord[2];
	struct snakeElem *prevElem;
	struct snakeElem *nextElem;
} snakeElem;

// In dieser Funktion läuft das
// gesamte Snake-Spiel ab
void runSnake(){
	initSnake();
	snakeLevelLoop();
	exitSnake();
}


void initSnake(){

	// Init Screen
	clear();
	refresh();
	nodelay(stdscr, TRUE);

	// Set Variables
	player = '#';
	cake = '+';
	playerInput = 'a';
	lastInput = 'a';
	bg = ' ';
	lifes = 1;
	length = 0;
	cakeSet = 0;
	addOne = 2;
	counter = 0;
	gameSpeed = 66;
	maxLength = LINES*COLS;

	headPos[0] = LINES/2;
	headPos[1] = COLS/2;

	snakeElem headElem;
	headElem.coord[0] = headPos[0];
	headElem.coord[1] = headPos[1];


	srand(time(NULL));


	//Setzt den Spieler in die Mitte
	mvaddch(headPos[0], headPos[1], player);
}

// Soland man ein Leben hat (=nicht kollidiert) oder
// der Spieler nicht q drück läuft dieser Loop
void snakeLevelLoop(){

	while( (lifes != 0) && ( (playerInput=getch()) != 'q') ){
		counter++;
		counter%maxLength;
		moveHead();
		deleteTail();
		setCake();
		refresh();
		napms(gameSpeed);
	}
}

// Wertet Spielereingabe aus und Zeichnet den neuen Kopf bzw
// zieht ein Leben ab und beendet somit den Game-Loop
// Bei keine Spielerinput nimmt den lastInput
void moveHead(){

	if( (playerInput != 'w')
		&& (playerInput != 'a')
		&& (playerInput != 's')
		&& (playerInput != 'd') ){
			playerInput = lastInput;
		}

	// Checkt nach den Richtungen und ob Terminalkante erreicht
	if(playerInput == 'a' && lastInput != 'd'){
		gameSpeed = 66;
		if(headPos[1] == 0){

			mvaddch(headPos[0], headPos[1], bg);
			lifes--;
		} else {
			headPos[1]--;
			checkNextField();
			mvaddch(headPos[0], headPos[1], player);
		}
		lastInput = 'a';
	}else if(playerInput == 'd' && lastInput != 'a'){
		gameSpeed = 66;
		if(headPos[1] == COLS-1){
			mvaddch(headPos[0], headPos[1], bg);
			lifes--;
		} else {
			headPos[1]++;
			checkNextField();
			mvaddch(headPos[0], headPos[1], player);
		}
		lastInput = 'd';
	}else if(playerInput == 'w' && lastInput != 's'){
		gameSpeed = 180;
		if(headPos[0] == 0){
			mvaddch(headPos[0], headPos[1], bg);
			lifes--;
		} else {
			headPos[0]--;
			checkNextField();
			mvaddch(headPos[0], headPos[1], player);
		}
		lastInput = 'w';

	}else if(playerInput == 's' && lastInput != 'w'){
		gameSpeed = 180;
		if(headPos[0] == LINES-1){
			mvaddch(headPos[0], headPos[1], bg);
			lifes--;
		} else {
			headPos[0]++;
			checkNextField();
			mvaddch(headPos[0], headPos[1], player);
		}
		lastInput = 's';
	}
}

void deleteTail(){

}

void setCake(){

	if(cakeSet == 0){
		int line = rand()%LINES;
		int col = rand()%COLS;

		while ( (mvinch( line, col) & A_CHARTEXT) == player){
			line = rand()%LINES;
			col = rand()%COLS;
		}
		mvaddch(line, col, cake);
		cakeSet = 1;
	}
}

void checkNextField(){
	if ( (mvinch(headPos[0], headPos[1]) & A_CHARTEXT) == cake){
		length++;
		cakeSet = 0;
	}
	if ((mvinch(headPos[0], headPos[1]) & A_CHARTEXT) == player){
		lifes--;
	}
}

void exitSnake(){
	nodelay(stdscr, FALSE);
}
