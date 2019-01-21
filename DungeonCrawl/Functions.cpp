//#pragma comment(lib, "winmm.lib")
#include "Functions.h"
#include "Game.h"
#include "resource.h"

#include <iostream>	//cout
#include <thread>	//this_thread::sleep_for
#include <chrono>	//chrone::seconds
#include <windows.h>	//needed for mmsystem.h
#include <mmsystem.h>	//this is for playing sounds


using std::cout;
using std::cin;

void displayIntro()
{
	cout << "Welcome to the Dungeon Crawler game!\n"
		<< "It's simple:\n"
		<< "\t-Start fresh each run with 100/100 health and base 15 stats\n"
		<< "\t-Get loot from kills/rooms\n"
		<< "\t-Encounters are random\n"
		<< "\t-If your health is 100 or higher, you cannot heal further\n"
		<< "\t-You cannot buy stat boosts past level 35\n"
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
		PlaySound(MAKEINTRESOURCE(IDR_WAVE3), NULL, SND_RESOURCE | SND_ASYNC);//IDR_WAVE3 is from the resource file and is the victory song
		system("pause");
	}
	else
	{
		std::cout << "Game over!\n";
		PlaySound(MAKEINTRESOURCE(IDR_WAVE4), NULL, SND_RESOURCE | SND_ASYNC);//IDR_WAVE4 is game over
		system("pause");
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
