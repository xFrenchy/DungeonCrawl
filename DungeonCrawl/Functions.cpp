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
		<< "\t-Start fresh each run with 100/120 health and base 15 stats\n"
		<< "\t-Get loot from kills/rooms\n"
		<< "\t-Encounters are random\n"
		<< "\t-If your health is 120 or higher, you cannot heal further\n"
		<< "\t-You cannot buy stat boosts past level 50\n"
		<< "\t-Make it through all the rooms and you win\n\n";
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

//displays a game over message and pauses for 1 second
void gameOver(EGameStatus status)
{
	if (status == EGameStatus::User_won)
	{
		cout << "Congratulations! You won!\n";
	}
	else
	{
		std::cout << "Game over!\n";
	}
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

bool isValidYNSI(char answer)
{
	answer = toupper(answer);
	if (answer == 'Y'){return true;}
	else if (answer == 'N'){return true;}
	else if (answer == 'S'){return true;}
	else if (answer == 'I'){return true;}
	else
	{
		cout << "Invalid answer, try again\n";
		cin.ignore(255, '\n');
		cin.clear();
		return false;
	}
	return false;
}

//forces the user to enter a 1 or 2
int intOneorTwo()
{
	int choice;
	bool valid = false;
	do
	{
		cin >> choice;
		if (cin.fail())	//if cin was not an int
		{
			cout << "\nThat was not an int!\nTry again:";
			cin.clear();
			cin.ignore(255, '\n');
		}
		//else the input was an int
		else if (choice == 1)
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
