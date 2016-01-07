#include "player.h"
#include "tunnel.h"
#include "level.h"
#include <ncurses.h>
#include <stdlib.h>

void movePlayer();
int getLifes();

static char playerInput;
static char lastInput;
static int playerSpawnPointLine;
static int playerSpawnPointCol;
static int playerPos[2];
static int lifes;
static int godMode;

// Setzt die Spielerdaten
void initPlayer(){
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
	mvaddch(playerPos[0], playerPos[1], ' ');

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

	}else if((playerInput == 'w') && (playerPos[0] != 1)){
		playerPos[0]--;
		lastInput = 'w';

	}else if((playerInput == 's') && (playerPos[0] != (LINES))){
		playerPos[0]++;
		lastInput = 's';

	}else if(playerInput == 'h'){
		godMode *= -1;

	}else if(playerInput == ' '){
		lastInput = ' ';
	}

	movePlayer();

}

void movePlayer(){

	// Checkt ob das Feld frei ist oder der Spieler stirbt
	if ( ((mvinch(playerPos[0]-1, playerPos[1]) & A_CHARTEXT) != ' ') && (godMode != 1) ){
		for(int i = 0; i < 5; i++){

			mvaddch(playerPos[0]-1, playerPos[1], 'x');
			refresh();
			napms(100);

			mvaddch(playerPos[0]-1, playerPos[1], ' ');
			refresh();
			napms(100);
		}

		for(int i = 0; i < COLS; i++){
			if( (mvinch(playerSpawnPointLine, i) & A_CHARTEXT) == ' '){
				for(int a = i; a < COLS; a++){
					if( ((mvinch(playerSpawnPointLine, a) & A_CHARTEXT) == '#') || (a == (COLS-1))){
						playerSpawnPointCol = i + ((a-i)/2);
						break;
					}
				}
				break;
			}
		}

		(lifes)--;
		playerPos[0] = playerSpawnPointLine;
		playerPos[1] = playerSpawnPointCol;
		mvaddch(playerPos[0]-1, playerPos[1], 'W');
	}else{
		mvaddch(playerPos[0]-1, playerPos[1], 'W');
	}
}

int getLifes(){
	return lifes;
}
