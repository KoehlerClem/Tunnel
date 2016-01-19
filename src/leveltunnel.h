#ifndef LEVELTUNNEL_H
#define LEVELTUNNEL_H

void runLevel(char gameModeChoice);
int getTotalLines();
int * getLineEdges(int line);
int getGameSpeed();
void setGameSpeed(int newGameSpeed);

extern char background;
extern char foreground;
#endif
