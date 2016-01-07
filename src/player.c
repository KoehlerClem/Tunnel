#include "player.h"
#include "game.h"
#include "leveltunnel.h"
#include <ncurses.h>
#include <stdlib.h>

void movePlayer();
int getLifes();

static char playerSpaceShip;
static char playerInput;
static char lastInput;
static int playerSpawnPointLine;
static int playerSpawnPointCol;
static int playerPos[2];
static int lifes;
static int godMode;

// Setzt die Spielerdaten
void initPlayer(){
	playerSpaceShip = 'A';
	playerInput = 'a';
	lastInput = ' ';
	playerSpawnPointLine = (LINES-4);
	playerSpawnPointCol = (COLS/2);
	playerPos[0] = playerSpawnPointLine;
	playerPos[1] = playerSpawnPointCol;
	lifes = 5;
	godMode = -1;
}

void printPlayerAction(char playerInput){

	// löscht die alte Spielerfigur
	mvaddch(playerPos[0], playerPos[1], background);

	// checkt ob ein Spielerinput vorliegt und
	// fals nicht wiederholt die letzte Bewegung
	if(playerInput == ERR){
		playerInput = lastInput;
	}

	// Checkt nach den Richtungen und ob Terminalkante erreicht
	if((playerInput == 'a') && (playerPos[1] != 0)){
		playerPos[1]--;
		lastInput = 'a';

	}else if((playerInput == 'd') && (playerPos[1] != COLS-1)){
		playerPos[1]++;
		lastInput = 'd';

	}else if((playerInput == playerSpaceShip) && (playerPos[0] != 1)){
		playerPos[0]--;
		lastInput = playerSpaceShip;

	}else if((playerInput == 's') && (playerPos[0] != (LINES))){
		playerPos[0]++;
		lastInput = 's';

	}else if(playerInput == 'h'){
		godMode *= -1;

	}else if(playerInput == background){
		lastInput = background;
	}

	movePlayer();

}

void movePlayer(){

	// Checkt ob das Feld frei ist oder der Spieler stirbt
	if ( ((mvinch(playerPos[0]-1, playerPos[1]) & A_CHARTEXT) != background) && (godMode != 1) ){
		for(int i = 0; i < 5; i++){

			mvaddch(playerPos[0]-1, playerPos[1], 'x');
			refresh();
			napms(100);

			mvaddch(playerPos[0]-1, playerPos[1], background);
			refresh();
			napms(100);
		}

		int *edges = getLineEdges(playerSpawnPointLine);
		playerSpawnPointCol = edges[0] + ((edges[1]-edges[0])/2);

		(lifes)--;
		playerPos[0] = playerSpawnPointLine;
		playerPos[1] = playerSpawnPointCol;
		mvaddch(playerPos[0]-1, playerPos[1], playerSpaceShip);
	}else{
		mvaddch(playerPos[0]-1, playerPos[1], playerSpaceShip);
	}
}

int getLifes(){
	return lifes;
}
