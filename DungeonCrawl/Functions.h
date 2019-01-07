#pragma once
#include "Game.h"

void displayIntro();
EGameStatus askToPlayGame();
void gameOver(EGameStatus);
bool isValidYesNo(char);
bool isValidYNSI(char);
int intOneorTwo();