#include "Functions.h"
#include "Game.h"
#include "resource.h"

#include <iostream>	//cout, cin
#include <cstdlib>	//srand, rand
#include <time.h>	//time
#include <windows.h>	//this is for execute a piece of code until keypress
#include <mmsystem.h>	//this is for playing sounds



using std::cout;

int main()
{
	srand(time(NULL));	//seeding for randum number
	EGameStatus status;
	displayIntro();
	do
	{
		//https://stackoverflow.com/questions/36424691/playsound-works-in-visual-studio-but-not-in-standalone-exe
		//https://stackoverflow.com/questions/33001284/incompatible-with-parameter-of-type-lpcwstr
		PlaySound(MAKEINTRESOURCE(IDR_WAVE1), NULL, SND_RESOURCE | SND_ASYNC);//IDR_WAVE1 is from the resource file and is the adventure song
		status = askToPlayGame();
	} while (status == EGameStatus::Invalid_guess);
	if (status == EGameStatus::EndGame)
	{
		gameOver(status);
		return 0;
	}
	else	//else we play the game
	{
		Game gameObj;
		do
		{
			status = gameObj.playGame();
		} while (status != EGameStatus::EndGame && status != EGameStatus::User_won);
		//once we are outside, we have end game status
		gameOver(status);	//displays a game over message and pauses for 1 second
		//show end stats
		gameObj.showEndStats();
		return 0;
	}
}

