#include <ncurses.h>

void playerAction(int playerPos[2], char playerKey, char *lastInput, int *lifes, int *score, int totalLines, int playerSpawnPointCol[], int *godMode);
void makeTunnel(int *tunnelDirection, int *tunnelEdgeLeft, int *tunnelEdgeRight, int *totalLines, int *score, int playerSpawnPointCol[]);
void printScore(int lines, int score, char scoreLevelBuffer[31]);
char startMenu();

int main(void){

	// Levelscreen
	initscr();
	noecho();
	curs_set(0);
	cbreak();


	while(startMenu() != 'q'){
		clear();
		refresh();
		nodelay(stdscr, TRUE);
		// Playersettings/daten
		char playerInput = 'a';
		char lastInput = 'a';
		int playerSpawnPointLine = (LINES-4);
		int playerSpawnPointCol[playerSpawnPointLine];
		for(int i = 0; i < playerSpawnPointLine; i++){
			playerSpawnPointCol[i] = (COLS/2);
		}
		int playerPos[2] = { playerSpawnPointLine, playerSpawnPointCol[0]};
		int gameSpeed = 30;
		int lifes = 1;
		int score = 0;
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



		while((playerInput = getch()) != 'm'){

			if(playerInput == 'p'){
				nodelay(stdscr, TRUE);
				while((playerInput = getch()) == ERR){
				}
			}


			makeTunnel(&tunnelDirection, &tunnelEdgeLeft, &tunnelEdgeRight, &totalLines, &score, playerSpawnPointCol);
			printScore(totalLines, score, scoreLevelBuffer);
			playerAction(playerPos, playerInput, &lastInput, &lifes, &score, totalLines, playerSpawnPointCol, &godMode);
			refresh();
			napms(gameSpeed);
		}
		nodelay(stdscr, FALSE);
	}
	endwin();
	return 0;
}

void playerAction(int playerPos[2], char playerKey, char *lastInput, int *lifes, int *score, int totalLines, int playerSpawnPointCol[(LINES-4)], int *godMode){
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

			mvaddch(playerPos[0]-1, playerPos[1], 'W');
			refresh();
			napms(100);

			mvaddch(playerPos[0]-1, playerPos[1], ' ');
			refresh();
			napms(100);
		}
		(*lifes)--;
		*score = 0;
		playerPos[0] = (LINES-4);
		playerPos[1] = ( playerSpawnPointCol[( totalLines % (LINES-4) )] );
		mvaddch(playerPos[0]-1, playerPos[1], 'W');

	}else{
		mvaddch(playerPos[0]-1, playerPos[1], 'W');
	}

}

void makeTunnel(int *tunnelDirection, int *tunnelEdgeLeft, int *tunnelEdgeRight, int *totalLines, int *score, int playerSpawnPointCol[LINES-4]){

	move(LINES-1, 0);
	deleteln();
	move(0,0);
	insertln();
	int colBuffer = ( (*totalLines) + (LINES-5)) % (LINES-4);
	int colBuffer2 = ( (*totalLines) + (LINES-6)) % (LINES-4);

	(*totalLines)++;
	(*score)++;

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
	}else{ // nach Rechts
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
	}
}

void printScore(int lines, int score, char scoreLevelBuffer[31]) {
	mvprintw(1, (COLS-33), scoreLevelBuffer);
	mvinstr(0, (COLS-33), scoreLevelBuffer);
	mvprintw(0, (COLS-33), "    Lines: %4i Score: %4i    ", lines, score);
}

char startMenu(){
	clear();
	refresh();
	nodelay(stdscr, FALSE);
	char gameName[] = "Tunnel (anykey)";
	int gameNameLength = sizeof(gameName);
	mvprintw( (LINES/2), ((COLS/2)-(gameNameLength/2)), gameName);
	char quit[] = "quit (q)";
	int quitLength = sizeof(quit);
	mvprintw( ( (LINES/2) + 1), ((COLS/2)-(quitLength/2)), quit);
	return getch();
}
