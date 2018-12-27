#include "Game.h"
#include "Functions.h"

#include <iostream>
#include <thread>	//this_thread::sleep_for
#include <chrono>	//chrone::seconds
#include <cstdlib>	//rand

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
		cout << "\nYou are currently on room " << currentRoomNumber << " out of " << maxRooms << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));
		//TODO create the actual playable part
		/*
		* Generate encounter
		* Run correct code based on encounter
		*/
		ETypeOfEncounter encounter;
		dungeonRoom.generateRoomType();
		encounter = dungeonRoom.getRoomType();
		switch (encounter)
		{
		case ETypeOfEncounter::Boss:
			//execute code for boss room
			break;
		case ETypeOfEncounter::Empty:
			//execute code for empty room
			dungeonRoom.emptyRoom();
			break;
		case ETypeOfEncounter::Minion:
			//execute code for minion room
			break;
		case ETypeOfEncounter::Shop:
			//execute code for shop room
			break;
		case ETypeOfEncounter::Treasure:
			//execute code for treasure room
			break;
		default:
			cout << "A non recognizable encouter happened, this is an error!\n";
			break;
		}

		//once we're done with the playable part, if possible we move on to the next room
		if (player.getHealth() <= 0)
		{
			return EGameStatus::EndGame;
		}
		else	//else we move forward
		{
			//prompt the user if they would like to move forward
			if (currentRoomNumber == maxRooms)	//Move the user forward automatically since they won
			{
				currentRoomNumber++;
				return EGameStatus::Move_forward;
			}
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
	roomType = ETypeOfEncounter::Empty;
}

void Dungeon::generateRoomType()
{
	int result = rand() % amountOfEncounterTypes;
	switch (result)
	{
	case 0:
		roomType = ETypeOfEncounter::Boss;
		break;
	case 1:
		roomType = ETypeOfEncounter::Empty;
		break;
	case 2:
		roomType = ETypeOfEncounter::Minion;
		break;
	case 3:
		roomType = ETypeOfEncounter::Shop;
		break;
	case 4:
		roomType = ETypeOfEncounter::Treasure;
		break;
	default:
		cout << "Something went wrong while generating encounter!\n";
		roomType = ETypeOfEncounter::Empty;
		break;
	}
}

void Dungeon::emptyRoom()
{
	cout << "\nThis rooms seems to be empty...Nothing to do here.\n";
	//TODO ask the user if they want to use their inventory before proceeding
	std::this_thread::sleep_for(std::chrono::seconds(1));
	return;
}

Player::Player()
{
	health = 100;
}
