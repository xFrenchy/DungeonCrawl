#include "Functions.h"
#include "Game.h"

#include <iostream>	//cout, cin
#include <cstdlib>	//srand, rand
#include <time.h>	//time

using std::cout;

int main()
{
	srand(time(NULL));	//seeding for randum number
	EGameStatus status;
	displayIntro();
	do
	{
		status = askToPlayGame();
	} while (status == EGameStatus::Invalid_guess);
	if (status == EGameStatus::EndGame)
	{
		gameOver();
		return 0;
	}
	else	//else we play the game
	{
		Game gameObj;
		do
		{
			status = gameObj.playGame();
		} while (status != EGameStatus::EndGame);
		//once we are outside, we have end game status
		gameOver();	//displays a game over message and pauses for 1 second
		//show end stats
		gameObj.showEndStats();
		return 0;
	}
}

