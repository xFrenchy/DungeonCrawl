#include "Functions.h"
#include "Game.h"

#include <iostream>	//cout
#include <thread>	//this_thread::sleep_for
#include <chrono>	//chrone::seconds

using std::cout;
using std::cin;

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
	cin >> answer;
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
		cin.ignore(255, '\n');
		cin.clear();
		return EGameStatus::Invalid_guess;
	}
}

void gameOver()
{
	std::cout << "Game over!\n";
	std::this_thread::sleep_for(std::chrono::seconds(1));
}

bool isValidYesNo(char letter)
{
	letter = toupper(letter);
	if (letter == 'Y')
	{
		return true;
	}
	else if (letter == 'N')
	{
		return true;
	}
	else
	{
		cout << "Invalid answer, try again\n";
		cin.ignore(255, '\n');
		cin.clear();
		return false;
	}
	return false;
}

int intOneorTwo()
{
	int choice;
	bool valid = false;
	do
	{
		cin >> choice;
		if (cin.fail())	//if cin was not an int
		{
			cout << "That was not an int!\n";
			cin.ignore(255, '\n');
			cin.clear();
		}
		//else the input was an int
		if (choice == 1)
		{
			valid = true;
			return choice;
		}
		else if (choice == 2)
		{
			valid = true;
			return choice;
		}
		else
		{
			cout << "Invalid number! You must choose 1 or 2, try again\n";
			valid = false;
		}
	} while (valid == false);

	return choice;
}
