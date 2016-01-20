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
static int shieldHP;
static int diplayNewShip;

// Setzt die Spielerdaten
void initPlayer(){
	playerSpaceShip = 'w';
	playerInput = 'a';
	lastInput = 'z';
	playerSpawnPointLine = (LINES-4);
	playerSpawnPointCol = (COLS/2);
	playerPos[0] = playerSpawnPointLine;
	playerPos[1] = playerSpawnPointCol;
	lifes = 1;
	godMode = 0;
	shieldHP = 0;
	diplayNewShip = 200;
}

void printPlayerAction(char playerInput){

	// löscht die alte Spielerfigur
	mvaddch(playerPos[0], playerPos[1], background);


	// Checkt nach den Richtungen und ob Terminalkante erreicht
	if((playerInput == 'a') && (playerPos[1] > 0)){
		if( (lastInput == 'a') && (playerPos[1] > 1) ){
			playerPos[1] -= 2;
		} else {
			playerPos[1]--;
		}
		lastInput = 'a';
	}else if((playerInput == 'd') && (playerPos[1] < COLS-1)){
		if((lastInput == 'd') && (playerPos[1] < COLS-2)){
			playerPos[1] += 2;
		} else {
			playerPos[1]++;
		}
		lastInput = 'd';
	}else if((playerInput == 'w') && (playerPos[0] != 1)){
		playerPos[0]--;
	}else if((playerInput == 's') && (playerPos[0] != (LINES))){
		playerPos[0]++;
	}else if(playerInput == 'h'){
		godMode *= -1;
	}else if((lastInput == 'a') && (playerPos[1] > 0)){
		playerPos[1]--;
	}else if((lastInput == 'd') && (playerPos[1] < COLS-1)){
		playerPos[1]++;
	}
	if(playerInput == ' '){
		int gameSpeed = getGameSpeed();
		if(shieldHP < 150){
			setGameSpeed ( (gameSpeed=gameSpeed+12) );
		}else{
			setGameSpeed( (gameSpeed=gameSpeed-7) );
		}
	}
	movePlayer();
}

void movePlayer(){

	if( (getTotalLines()%250) == 0 ){
		shieldHP += 50;
		godMode = 1;
	}
	if( (getTotalLines()%5000) == 0 ){
		lifes++;
	}
	if( (shieldHP >= 150) && ((playerSpaceShip == 'x') || (diplayNewShip > 0)) ){
		playerSpaceShip = 'A';
		char shipUpgrade[] = " !! SHIELDLEVEL SECURE - TURBO ON  !!  ";
		int shipUpgradeLength = sizeof(shipUpgrade);
		mvprintw( 2, ((COLS/2)-(shipUpgradeLength/2)), shipUpgrade);
		diplayNewShip--;
	}else if(diplayNewShip<200 && shieldHP<150){
		playerSpaceShip = 'x';
		char shipUpgrade[] = " !! SHIELDLEVEL CRITICAL - TURBO OFF  !! ";
		int shipUpgradeLength = sizeof(shipUpgrade);
		mvprintw( 2, ((COLS/2)-(shipUpgradeLength/2)), shipUpgrade);
		diplayNewShip++;
	}

	char nextPos = ( mvinch(playerPos[0]-1, playerPos[1]) & A_CHARTEXT);

	// Checkt ob das Feld frei ist oder der Spieler stirbt
	// crash mit background
	if ( (nextPos == foreground) && (godMode == (0) ) ){
		// lässt den Spieler aufblinken
		for(int i = 0; i < 5; i++){
			mvaddch(playerPos[0]-1, playerPos[1], 'x');
			refresh();
			napms(100);
			mvaddch(playerPos[0]-1, playerPos[1], background);
			refresh();
			napms(100);
		}
		int *edges = getLineEdges(playerSpawnPointLine-2);
		playerSpawnPointCol = edges[0] + ((edges[1]-edges[0])/2);
		lifes--;
		playerPos[0] = playerSpawnPointLine;
		playerPos[1] = playerSpawnPointCol;
		mvaddch(playerPos[0]-1, playerPos[1], playerSpaceShip);
	}else if( (nextPos == foreground) && (godMode == 1) ){
		shieldHP -= 4;
		if(shieldHP < 1){
			shieldHP = 0;
			godMode = 0;
		}
		mvaddch(playerPos[0]-1, playerPos[1], playerSpaceShip);
	// goodie
	}else if(nextPos == '+'){
		shieldHP +=2;
		godMode = 1;
		mvaddch(playerPos[0]-1, playerPos[1], playerSpaceShip);
	// baddie
	}else if(nextPos == '-'){
		shieldHP--;
		if(shieldHP < 1){
			shieldHP = 0;
			godMode = 0;
		}
		mvaddch(playerPos[0]-1, playerPos[1], playerSpaceShip);
	}else{
		mvaddch(playerPos[0]-1, playerPos[1], playerSpaceShip);
	}
}


int getLifes(){
	return lifes;
}

int getShieldHP(){
	return shieldHP;
}
