#include "Game.h"

#include <iostream>

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
		//TODO create the actual playable part


		//once we're done with the playable part, if possible we move on to the next room
		if (false)	//TODO change this with if player is dead
		{
			return EGameStatus::EndGame;
		}
		else	//else we move forward
		{
			currentRoomNumber++;
			return EGameStatus::Move_forward;
		}
	}
	else	//else we have gone through every single room
	{
		cout << "You have reached the end!\n";
		return EGameStatus::EndGame;
	}

}

Dungeon::Dungeon()
{
	roomType = ETypeOfEncounter::Minion;
}
