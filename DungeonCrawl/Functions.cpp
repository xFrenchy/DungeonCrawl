#include "Functions.h"
#include "Game.h"

#include <iostream>	//cout
#include <thread>	//this_thread::sleep_for
#include <chrono>	//chrone::seconds

using std::cout;

void displayIntro()
{
	cout << "Welcome to the Dungeon Crawler game!\n"
		<< "It's simple:\n"
		<< "\t1)Start fresh\n"
		<< "\t2)Get loot from kills/rooms\n"
		<< "\t3)Encounters are random\n"
		<< "\t4)Make it through all the rooms and you win\n\n";
	return;
}

EGameStatus askToPlayGame()
{
	cout << "Would you like to play?<Y/N>";
	char answer;
	std::cin >> answer;
	answer = toupper(answer);
	if (answer == 'Y')
	{
		return EGameStatus::Play;
	}
	else if (answer == 'N')
	{
		return EGameStatus::EndGame;
	}
	else
	{
		cout << "Invalid answer, try again\n";
		std::cin.ignore(255, '\n');
		std::cin.clear();
		return EGameStatus::Invalid_guess;
	}
}

void gameOver()
{
	std::cout << "Game over!\n";
	std::this_thread::sleep_for(std::chrono::seconds(2));
}
