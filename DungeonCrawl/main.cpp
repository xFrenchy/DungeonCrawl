#include "Functions.h"
#include "Game.h"

#include <iostream>	//cout & cin
#include <thread>	//this_thread::sleep_for
#include <chrono>	//chrone::seconds

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
		std::cout << "Game over!\n";
		std::this_thread::sleep_for(std::chrono::seconds(2));
		return 0;
	}
	else	//else we play the game
	{
		
	}
}

