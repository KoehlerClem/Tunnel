#include "tunnel.h"

void playerAction(int playerPos[2], char playerKey, char *lastInput, int *lifes, int totalLines, int playerSpawnPointCol[], int *godMode);
void makeTunnel(int *tunnelDirection, int *tunnelEdgeLeft, int *tunnelEdgeRight, int *totalLines, int playerSpawnPointCol[]);
void printScore(int lines, char scoreLevelBuffer[31], int lifes);
char startMenu();
void randomMode(int *tunnelDirection);

int main(void){

	// Levelscreen
	initscr();
	noecho();
	curs_set(0);
	cbreak();
	int gameMode;
    //game loop
	while((gameMode = startMenu()) != 'q'){
		clear();
		refresh();
		nodelay(stdscr, TRUE);
		// Playersettings/daten
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

		// Tunnelsettings/daten
		int tunnelEdgeLeft = COLS * 0.4;
		int tunnelEdgeRight = COLS * 0.6;
		int tunnelDirection = 0;
		int totalLines = 0;
		char scoreLevelBuffer[31] = {
			"###############################"
		};
		//Starttunnel
		for(int i = 0; i < LINES; i++){
			for(int x = 0; x < COLS; x++){
				if( (tunnelEdgeLeft < x) && (tunnelEdgeRight > x) ){
					mvaddch(i, x, ' ');
				}else{
					mvaddch(i, x, '#');
				}
			}
		}
		refresh();

		// GAME LOOP
		while( ( (playerInput = getch()) != 'm') && (lifes > -1)){

			if(playerInput == 'p'){
				nodelay(stdscr, FALSE);
				playerInput = getch();

			}
			nodelay(stdscr, TRUE);

			makeTunnel(&tunnelDirection, &tunnelEdgeLeft, &tunnelEdgeRight, &totalLines, playerSpawnPointCol);
			if(gameMode == 'r') {
				randomMode(&tunnelDirection);
			}
			printScore(totalLines, scoreLevelBuffer, lifes);
			playerAction(playerPos, playerInput, &lastInput, &lifes, totalLines, playerSpawnPointCol, &godMode);
			refresh();
			napms(gameSpeed);
		}
		nodelay(stdscr, FALSE);

		FILE *f = fopen("hightscore.txt", "a");
		if (f == NULL)
		{
		    printf("Error opening file!\n");
		    exit(1);
		}
		int i = 1;
		fprintf(f, "Player: %-20s Hightscore: %09i\n", "Testplayer",totalLines);
		fclose(f);


	}
	endwin();
	return 0;
}

void playerAction(int playerPos[2], char playerKey, char *lastInput, int *lifes, int totalLines, int playerSpawnPointCol[(LINES-4)], int *godMode){
	// löscht die alte Spielerfigur
	mvaddch(playerPos[0], playerPos[1], ' ');

	// checkt ob ein Spielerinput vorliegt
	// wenn ja, übernimmt diesen als Spielerinput
	// und wiederholt nicht die letzte Eingabe
	char playerInput;
	if(playerKey != ERR){
		playerInput = playerKey;
	}else{
		playerInput = *lastInput;
	}

	// checkt nach den Richtungen und ob kante erreicht
	if((playerInput == 'a') && (playerPos[1] != 0)){
		playerPos[1]--;
		*lastInput = 'a';

	}else if((playerInput == 'd') && (playerPos[1] != COLS-1)){
		playerPos[1]++;
		*lastInput = 'd';

	}else if((playerInput == 'w') && (playerPos[0] != 1)){
		playerPos[0]--;
		*lastInput = 'w';

	}else if((playerInput == 's') && (playerPos[0] != (LINES))){
		playerPos[0]++;
		*lastInput = 's';

	}else if(playerInput == 'h'){
		*godMode = (*godMode * (-1));

	}else if(playerInput == ' '){
		*lastInput = ' ';
	}

	if ( ((mvinch(playerPos[0]-1, playerPos[1]) & A_CHARTEXT) != ' ') && (*godMode != 1) ){
		for(int i = 0; i < 5; i++){

			mvaddch(playerPos[0]-1, playerPos[1], 'x');
			refresh();
			napms(100);

			mvaddch(playerPos[0]-1, playerPos[1], ' ');
			refresh();
			napms(100);
		}
		(*lifes)--;
		playerPos[0] = (LINES-4);
		playerPos[1] = ( playerSpawnPointCol[( totalLines % (LINES-4) )] );
		mvaddch(playerPos[0]-1, playerPos[1], 'W');

	}else{
		mvaddch(playerPos[0]-1, playerPos[1], 'W');
	}

}

void makeTunnel(int *tunnelDirection, int *tunnelEdgeLeft, int *tunnelEdgeRight, int *totalLines, int playerSpawnPointCol[LINES-4]){

	move(LINES-1, 0);
	deleteln();
	move(0,0);
	insertln();
	int colBuffer = ( (*totalLines) + (LINES-5)) % (LINES-4);
	int colBuffer2 = ( (*totalLines) + (LINES-6)) % (LINES-4);

	(*totalLines)++;

	if(*tunnelDirection == 0){ // nach Links
		(*tunnelEdgeLeft)--;
		(*tunnelEdgeRight)--;
		playerSpawnPointCol[colBuffer] = (playerSpawnPointCol[colBuffer2] - 1);
		for(int i = 0; i < COLS; i++){
			if( (i > *tunnelEdgeLeft) && (i < *tunnelEdgeRight)){
				mvaddch(0, i, ' ');
			}else {
				mvaddch(0, i, '#');
			}
		}
		if(*tunnelEdgeLeft < 2) *tunnelDirection = 1;
	}else if (*tunnelDirection == 1){ // nach Rechts
		(*tunnelEdgeLeft)++;
		(*tunnelEdgeRight)++;
		playerSpawnPointCol[colBuffer] = (playerSpawnPointCol[colBuffer2] + 1);
		for(int i = 0; i < COLS; i++){
			if( (i > *tunnelEdgeLeft) && (i < *tunnelEdgeRight)){
				mvaddch(0, i, ' ');
			} else {
				mvaddch(0, i, '#');
			}
		}
		if(*tunnelEdgeRight > (COLS-3)) *tunnelDirection = 0;
	}else{ // Geradeaus
		playerSpawnPointCol[colBuffer] = playerSpawnPointCol[colBuffer2];
		for(int i = 0; i < COLS; i++){
			if( (i > *tunnelEdgeLeft) && (i < *tunnelEdgeRight)){
				mvaddch(0, i, ' ');
			} else {
				mvaddch(0, i, '#');
			}
		}
	}
}

void printScore(int lines, char scoreLevelBuffer[31], int lifes) {
	mvprintw(1, (COLS-33), scoreLevelBuffer);
	mvinstr(0, (COLS-33), scoreLevelBuffer);
	mvprintw(0, (COLS-33), "  Score: %09i Lifes: %02i  ", lines, lifes);
}

char startMenu(){
	clear();
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

void randomMode(int *tunnelDirection){
	switch (rand()% 35) {
		case 0: *tunnelDirection = 0; break;
		case 1: *tunnelDirection = 1; break;
		default: break;
	}
}
