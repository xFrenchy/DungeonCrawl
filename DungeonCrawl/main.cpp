#include "Functions.h"
#include "Game.h"

#include <iostream>	//cout & cin


using std::cout;

int main()
{
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
		gameOver();
		return 0;
	}
}

