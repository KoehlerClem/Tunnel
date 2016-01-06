#include "player.h"
#include "tunnel.h"
#include "level.h"
#include <ncurses.h>

void initPlayer();
void printPlayerAction(char playerInput);
void movePlayer();
int getLifes();

static char playerInput;
static char lastInput;
static int playerSpawnPointLine;
static int playerSpawnPointCol[];
static int playerPos[];
static int gameSpeed;
static int lifes;
static int godMode;

// Setzt die Spielerdaten
void initPlayer(){
	char playerInput = ' ';
	char lastInput = ' ';
	int playerSpawnPointLine = (LINES-4);
	int playerSpawnPointCol[playerSpawnPointLine];
	for(int i = 0; i < playerSpawnPointLine; i++){
		playerSpawnPointCol[i] = (COLS/2);
	}
	int playerPos[2] = { playerSpawnPointLine, playerSpawnPointCol[0]};
	int gameSpeed = 33;
	int lifes = 5;
	int godMode = -1;
}

void printPlayerAction(char playerInput){

	// löscht die alte Spielerfigur
	mvaddch(playerPos[0], playerPos[1], ' ');

	// checkt ob ein Spielerinput vorliegt und
	// fals nicht wiederholt die letzte Bewegung
	if(playerInput != ERR){
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
		(lifes)--;
		playerPos[0] = (playerSpawnPointLine);
		playerPos[1] = ( playerSpawnPointCol[( getTotalLines() % (playerSpawnPointLine) )] );
		mvaddch(playerPos[0]-1, playerPos[1], 'W');

	}else{
		mvaddch(playerPos[0]-1, playerPos[1], 'W');
	}
}

int getLifes(){
	return lifes;
}
