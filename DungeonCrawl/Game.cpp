#include "Game.h"
#include "Functions.h"

#include <iostream>
#include <thread>	//this_thread::sleep_for
#include <chrono>	//chrone::seconds

using std::cout;

Game::Game()
{
	gameOver = false;
	maxRooms = 5;	//TODO make this number depend on difficulty or simply just a value that is always different based on something
	currentRoomNumber = 1;
}

EGameStatus Game::playGame()
{
	if (currentRoomNumber <= maxRooms)
	{
		cout << "You are currently on room " << currentRoomNumber << " out of " << maxRooms << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));
		//TODO create the actual playable part


		//once we're done with the playable part, if possible we move on to the next room
		if (player.getHealth() <= 0)
		{
			return EGameStatus::EndGame;
		}
		else	//else we move forward
		{
			//prompt the user if they would like to move forward
			cout << "Would you like to move on to the next room?<Y/N>";
			bool valid = false;
			char answer = ' ';
			do
			{
				std::cin >> answer;
				answer = toupper(answer);
				valid = isValidYesNo(answer);
			} while (valid == false);
			//once outside the loop we know the answer types a yes or a no
			if (answer == 'Y')
			{
				currentRoomNumber++;
				return EGameStatus::Move_forward;
			}
			else if (answer == 'N')
			{
				return EGameStatus::EndGame;
			}
		}
	}
	else	//else we have gone through every single room
	{
		cout << "You have reached the end!\n\t#############\n\tYOU WON!\n\t#############\n\n";
		return EGameStatus::EndGame;
	}

}

Dungeon::Dungeon()
{
	roomType = ETypeOfEncounter::Minion;
}

Player::Player()
{
	health = 100;
}
