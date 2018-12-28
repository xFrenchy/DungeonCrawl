#include "Game.h"
#include "Functions.h"

#include <iostream>
#include <thread>	//this_thread::sleep_for
#include <chrono>	//chrone::seconds
#include <cstdlib>	//rand
#include <stdlib.h>	//clear screan system("CLS")

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
			dungeonRoom.minionRoom(player);
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
			cout << "Oh dear, you have died!\n";
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
	//TODO create rarity to rooms
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

void Dungeon::minionRoom(Player &p1)
{
	//Player will always go first
	Minion minion;	//minion spawned in the room
	int userChoice;
	cout << "You've encountered a minion!\n"
		<< "\nWould you like to try to fight or run?\n"
		<< "1)Fight\n"
		<< "2)Run\n";
	//check that userChoice is valid
	userChoice = intOneorTwo();	//this forces the user to type in a 1 or a 2 so I know userChoice is either a 1 or a 2 after that function
	if (userChoice == 2)
	{
		//attempt to escape
		bool hasEscaped = false;
		hasEscaped = minion.isEscape();
		if (hasEscaped == true)
		{
			return;
		}
	}
	//else we fight
	//TODO execute fight
	system("CLS");	//clears the screen
	cout << "Player HP: " << p1.getHealth() << "\t\t\tMinion HP: " << minion.getHealth() << std::endl;
	do
	{
		minion.setHealth(minion.getHealth() - p1.attack());	//player attacked the minion and updated health
		if (minion.getHealth() <= 0)
		{
			minion.setAlive(false);
		}
		if (minion.getIsAlive() == false)
		{
			system("CLS");
			cout << "Player HP: " << p1.getHealth() << "\t\t\tMinion HP: " << minion.getHealth() << std::endl;
			cout << "You defeated the minion!\n";
			break;
		}
		//End of turn for player attacking minion
		system("CLS");
		cout << "Player HP: " << p1.getHealth() << "\t\t\tMinion HP: " << minion.getHealth() << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(1200));

		p1.setHealth(p1.getHealth() - minion.attack());
		if (p1.getHealth() <= 0)
		{
			p1.setAlive(false);
		}
		system("CLS");
		cout << "Player HP: " << p1.getHealth() << "\t\t\tMinion HP: " << minion.getHealth() << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(1200));

	} while (p1.getIsAlive() && minion.getIsAlive());	//while p1 is alive and minion is alive, if one dies, fight is over
	return;
}

Player::Player()
{
	health = 100;
	isAlive = true;
}

int Player::attack()
{
	//Player's attack will vary only based on the stats the player has
	//TODO implement stats to affect player attacks
	return (rand() % 16);	//default range is between 0-15
}

Minion::Minion()
{
	setHealth(20);
	setAlive(true);
}

int Minion::attack()
{
	//A minion should hit low and should never be able to kill the player unless the player was low to begin with
	//A minion should be a slight annoyance with potential to have ok loot to compensate for
	//TODO make a more rich attack
	return (rand() % 11);	//minion can hit from 0-10
}

bool Minion::isEscape()
{
	//player will have a 25% chance of escaping any minion encounter
	int chance = (rand() % 4);	//range is 0-3
	if (chance == 0)
	{
		cout << "You have succefully escaped from the minion!\n\n";
		return true;
	}
	return false;	//else return false because the player did not manage to escape
}
