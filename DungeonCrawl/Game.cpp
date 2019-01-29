#include "Game.h"
#include "Functions.h"
#include "resource.h"

#include <iostream>
#include <thread>	//this_thread::sleep_for
#include <chrono>	//chrone::seconds
#include <cstdlib>	//rand
#include <stdlib.h>	//clear screan system("CLS")
#include <windows.h>	//this is for execute a piece of code until keypress
#include <mmsystem.h>	//this is for playing sounds

using std::cout;
using std::cin;

Game::Game()
{
	gameOver = false;
	maxRooms = 50;	//TODO make this number depend on difficulty or simply just a value that is always different based on something
	currentRoomNumber = 1;
	lastRoom = ETypeOfEncounter::Null;
}

EGameStatus Game::playGame()
{
	if (currentRoomNumber <= maxRooms)
	{
		//PlaySound(TEXT("Sounds\\AdventureSong.wav"), NULL, SND_FILENAME | SND_ASYNC);
		cout << "\nYou are currently on room " << currentRoomNumber << " out of " << maxRooms << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));
		//TODO create the actual playable part
		/*
		* Generate encounter
		* Run correct code based on encounter
		*/
		ETypeOfEncounter encounter;
		dungeonRoom.generateRoomType(lastRoom, currentRoomNumber);
		encounter = dungeonRoom.getRoomType();
		lastRoom = encounter;	//setting up last room for next turn
		switch (encounter)
		{
		case ETypeOfEncounter::Dragon:
			//execute code for Dragon room
			dungeonRoom.dragonRoom(player);
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
			dungeonRoom.shopRoom(player);
			break;
		case ETypeOfEncounter::Treasure:
			//execute code for treasure room
			dungeonRoom.treasureRoom(player, currentRoomNumber);
			break;
		case ETypeOfEncounter::SwarmOfMinions:
			dungeonRoom.swarmMinionRoom(player);
			break;
		case ETypeOfEncounter::SmallDragon:
			dungeonRoom.smallDragonRoom(player);
			break;
		default:
			cout << "A non recognizable encouter happened, this is an error!\n";
			break;
		}

		//once we're done with the playable part, if possible we move on to the next room
		if (player.getHealth() <= 0)
		{
			PlaySound(MAKEINTRESOURCE(IDR_WAVE17), NULL, SND_RESOURCE | SND_SYNC);//IDR_WAVE17 is the playerDeath, sync so you can hear it before gameOver
			cout << "Oh dear, you have died!\n";
			return EGameStatus::EndGame;
		}
		else	//else we move forward
		{
			//prompt the user if they would like to move forward or show stats
			if (currentRoomNumber == maxRooms)	//Move the user forward automatically since they won
			{
				//currentRoomNumber++;
				return EGameStatus::User_won;
			}
			bool moveForward = false;
			while (moveForward == false)
			{
				cout << "Y- Proceed to next room\nN- End game\nS- Show stats\nI- Show and use inventory\n";
				bool valid = false;
				char answer = ' ';
				do
				{
					std::cin >> answer;
					answer = toupper(answer);
					valid = isValidYNSI(answer);
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
				else if (answer == 'S')
				{
					player.displayStat();
					moveForward = false;
				}
				else if (answer == 'I')
				{
					player.showAndUseInvC();
					moveForward = false;
				}
			}
		}
	}
	else	//else we have gone through every single room, this should never execute
	{
		cout << "You have reached the end!\n\t#############\n\tYOU WON!\n\t#############\n\n";
		return EGameStatus::EndGame;
	}

}

//displays player ending health, attack, strength, defence
void Game::showEndStats()
{
	cout << "Your adventures are over, here are the stats you ended with";
	player.displayStat();
	cout << "Gold: " << player.getGold() << "\n";
	cout << "Last room: " << currentRoomNumber << "\n";
	std::this_thread::sleep_for(std::chrono::seconds(3));
}

Dungeon::Dungeon()
{
	roomType = ETypeOfEncounter::Empty;
}

//Generate a random room, cannot be previous room and checks on current room to see what it's allowed to generate
void Dungeon::generateRoomType(ETypeOfEncounter &lastRoom, int currentRoomNumber)
{
	//TODO create rarity to rooms
	bool validRoom = false;
	do
	{
		int result = rand() % amountOfEncounterTypes;
		switch (result)
		{
		case 0:
			roomType = ETypeOfEncounter::Dragon;
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
		case 5:
			roomType = ETypeOfEncounter::SwarmOfMinions;
			break;
		case 6:
			roomType = ETypeOfEncounter::SmallDragon;
			break;
		default:
			cout << "Something went wrong while generating encounter!\n";
			roomType = ETypeOfEncounter::Empty;
			break;
		}//end of switch case
		if (lastRoom != roomType)
		{
			if (currentRoomNumber == 10)	//first easy boss encounter
			{
				roomType = ETypeOfEncounter::SwarmOfMinions;
				validRoom = true;
			}
			else if (currentRoomNumber == 20)	//first medium boss encounter
			{
				roomType = ETypeOfEncounter::Dragon;
				validRoom = true;
			}
			//else if(currentRoomNumber == 30)	//first hard boss encounter
			else if (roomType == ETypeOfEncounter::SwarmOfMinions && currentRoomNumber < 10)	//cannot get swarm of minions before level 10
				validRoom = false;
			else if (roomType == ETypeOfEncounter::Dragon && currentRoomNumber < 20)
				validRoom = false;
			//else if()
			else	//else it's valid
			{
				validRoom = true;
			}
		}
	} while (validRoom == false);
	return;
}

void Dungeon::emptyRoom()
{
	PlaySound(MAKEINTRESOURCE(IDR_WAVE14), NULL, SND_RESOURCE | SND_ASYNC);//IDR_WAVE14 is empty room
	cout << "\nThis rooms seems to be empty...Nothing to do here.\n";
	//TODO implement skilling into this room
	std::this_thread::sleep_for(std::chrono::seconds(1));
	return;
}

void Dungeon::minionRoom(Player &p1)
{
	//Player will always go first
	Minion minion;	//minion spawned in the room
	bool playerSkipTurn = false;
	int userChoice;
	cout << "You've encountered a minion!\n";
	PlaySound(MAKEINTRESOURCE(IDR_WAVE5), NULL, SND_RESOURCE | SND_ASYNC);//IDR_WAVE5 is the minion encounter
	cout << "\t`oo.' \n"
		<< "\t`-')  ,.\n"
		<< "\t( `-'/^`\n"
		<< "\t-`J-d   \n";

	cout << "\nWould you like to try to fight or run?\n"
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
	system("CLS");	//clears the screen
	cout << "Player HP: " << p1.getHealth() << "\t\t\tMinion HP: " << minion.getHealth() << std::endl;
	do
	{
		if (playerSkipTurn == false)
		{
			PlaySound(MAKEINTRESOURCE(IDR_WAVE6), NULL, SND_RESOURCE | SND_SYNC);//IDR_WAVE6 is the playerHit
			minion.defend(p1.attack());	//player attacked the minion and updated health
			
		}
		else	//else it's true and the player skips a turn
		{
			cout << "\nYou just ate and can't hit this turn!\n";
			playerSkipTurn = false;
			std::this_thread::sleep_for(std::chrono::milliseconds(1500));
		}
		if (minion.getIsAlive() == false)	//if the minion died
		{
			//since the minion is dead, we're going to set the minion's health to 0 to prevent it from going into the negatives
			std::this_thread::sleep_for(std::chrono::milliseconds(1500));	//needed for player to see what they hit on the minion to kill it
			minion.setHealth(0);
			system("CLS");
			cout << "Player HP: " << p1.getHealth() << "\t\t\tMinion HP: " << minion.getHealth() << std::endl;
			cout << "You defeated the minion!\n";
			minion.deathLoot(p1);
			break;
		}

		//End of turn for player attacking minion
		std::this_thread::sleep_for(std::chrono::milliseconds(1500));
		system("CLS");
		cout << "Player HP: " << p1.getHealth() << "\t\t\tMinion HP: " << minion.getHealth() << std::endl;
		PlaySound(MAKEINTRESOURCE(IDR_WAVE8), NULL, SND_RESOURCE | SND_SYNC);//IDR_WAVE8 is the minionHit, this one is sync because if it was async
														//the player is hit sound will play almost instantly and you won't hear minion hit sound
		p1.defend(minion.attack());	//minion attacks the player
		PlaySound(MAKEINTRESOURCE(IDR_WAVE9), NULL, SND_RESOURCE | SND_SYNC);//IDR_WAVE9 is the playerIsHit
		//std::this_thread::sleep_for(std::chrono::milliseconds(1500));
		system("CLS");
		cout << "Player HP: " << p1.getHealth() << "\t\t\tMinion HP: " << minion.getHealth() << std::endl;
		//cin.clear();	//clears buffer before player is prompted if they want to go in their inventory
		//cin.ignore(255, '\n');	//this doesn't work right now
		if (p1.getHealth() > 0)
		{
			cout << "\nI- go inside your inventory!\n";
			std::this_thread::sleep_for(std::chrono::milliseconds(1500));
			if (GetAsyncKeyState(0x49))	//TODO change this key to 'I'
			{
				cin.ignore(255, '\n');
				cin.clear();
				//clear buffer so that it doesn't go inside the if statement if the user spams iiiii to get into it once
				playerSkipTurn = p1.showAndIsUseInvCFight();
			}
		}//if player is alive

	} while (p1.getIsAlive() && minion.getIsAlive());	//while p1 is alive and minion is alive, if one dies, fight is over
	return;
}

void Dungeon::dragonRoom(Player &p1)
{
	Dragon boss;	//boss spawned in the room
	bool playerSkipTurn = false;
	int userChoice;
	cout << "You've encountered a dragon!\n";
	PlaySound(MAKEINTRESOURCE(IDR_WAVE10), NULL, SND_RESOURCE | SND_ASYNC);//IDR_WAVE10 is the dragon encounter
	cout << "\t                                                 /===-_---~~~~~~~~~------____\n"
		<< "\t                                                |===-~___                _,-'\n"
		<< "\t                 -==\\\\                         `//~\\\\   ~~~~`---.___.-~~\n"
		<< "\t             ______-==|                         | |  \\\\           _-~`\n"
		<< "\t       __--~~~  ,-/-==\\\\                        | |   `\\        ,'\n"
		<< "\t    _-~       /'    |  \\\\                      / /      \\      /\n"
		<< "\t  .'        /       |   \\\\                   /' /        \\   /'\n"
		<< "\t /  ____  /         |    \\`\\.__/-~~ ~ \\ _ _/'  /          \\/'\n"
		<< "\t/-'~    ~~~~~---__  |     ~-/~         ( )   /'        _--~`\n"
		<< "\t                  \\_|      /        _)   ;  ),   __--~~\n"
		<< "\t                    '~~--_/      _-~/-  / \\   '-~ \\\n"
		<< "\t                   {\\__--_/}    / \\\\_>- )<__\\      \\\n"
		<< "\t                   /'   (_/  _-~  | |__>--<__|      | \n"
		<< "\t                  |0  0 _/) )-~     | |__>--<__|      |\n"
		<< "\t                  / /~ ,_/       / /__>---<__/      |  \n"
		<< "\t                 o o _//        /-~_>---<__-~      /\n"
		<< "\t                 (^(~          /~_>---<__-      _-~              \n"
		<< "\t                ,/|           /__>--<__/     _-~                 \n"
		<< "\t             ,//('(          |__>--<__|     /                  .----_ \n"
		<< "\t            ( ( '))          |__>--<__|    |                 /' _---_~\\\n"
		<< "\t         `-)) )) (           |__>--<__|    |               /'  /     ~\\`\\ \n"
		<< "\t        ,/,'//( (             \\__>--<__\\    \\            /'  //        ||\n"
		<< "\t      ,( ( ((, ))              ~-__>--<_~-_  ~--____---~' _/'/        /'\n"
		<< "\t    `~/  )` ) ,/|                 ~-_~>--<_/-__       __-~ _/ \n"
		<< "\t  ._-~//( )/ )) `                    ~~-'_/_/ /~~~~~~~__--~ \n"
		<< "\t   ;'( ')/ ,)(                              ~~~~~~~~~~ \n"
		<< "\t  ' ') '( (/\n"
		<< "\t    '   '  `   \n";


	cout << "\nWould you like to try to fight or run?\n"
		<< "1)Fight\n"
		<< "2)Run\n";
	//check that userChoice is valid
	userChoice = intOneorTwo();	//this forces the user to type in a 1 or a 2 so I know userChoice is either a 1 or a 2 after that function
	if (userChoice == 2)
	{
		//attempt to escape
		bool hasEscaped = false;
		hasEscaped = boss.isEscape();
		if (hasEscaped == true)
		{
			return;
		}
	}
	//else we fight
	system("CLS");	//clears the screen
	cout << "Player HP: " << p1.getHealth() << "\t\t\tDragon HP: " << boss.getHealth() << std::endl;
	do
	{
		if (playerSkipTurn == false)
		{
			PlaySound(MAKEINTRESOURCE(IDR_WAVE6), NULL, SND_RESOURCE | SND_SYNC);//IDR_WAVE6 is the playerHit, sync so that dragonBeingHit is heard after
			boss.defend(p1.attack());	//player attacked the dragon and updated health
			PlaySound(MAKEINTRESOURCE(IDR_WAVE11), NULL, SND_RESOURCE | SND_SYNC);//IDR_WAVE10 is the dragonBeingHit
		}
		else	//else it's true and the player skips a turn
		{
			cout << "\nYou just ate and can't hit this turn!\n";
			playerSkipTurn = false;
			std::this_thread::sleep_for(std::chrono::milliseconds(1500));
		}
		if (boss.getIsAlive() == false)	//if dragon is dead
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1500));	//needed so player sees what they hit to kill the dragon
			boss.setHealth(0);
			system("CLS");
			cout << "Player HP: " << p1.getHealth() << "\t\t\tDragon HP: " << boss.getHealth() << std::endl;
			cout << "You defeated the dragon!\n";
			boss.deathLoot(p1);
			break;
		}

		//End of turn for player attacking dragon
		std::this_thread::sleep_for(std::chrono::milliseconds(1500));
		system("CLS");
		cout << "Player HP: " << p1.getHealth() << "\t\t\tDragon HP: " << boss.getHealth() << std::endl;
		PlaySound(MAKEINTRESOURCE(IDR_WAVE13), NULL, SND_RESOURCE | SND_SYNC);//IDR_WAVE13 is the dragonHit, sync to hear it before playerIsHit
		p1.defend(boss.attack());	//dragon attacks player
		PlaySound(MAKEINTRESOURCE(IDR_WAVE9), NULL, SND_RESOURCE | SND_SYNC);//IDR_WAVE9 is the playerIsHit
		//std::this_thread::sleep_for(std::chrono::milliseconds(1500));
		system("CLS");
		cout << "Player HP: " << p1.getHealth() << "\t\t\tDragon HP: " << boss.getHealth() << std::endl;
		//cin.clear();	//clears buffer before player is prompted if they want to go in their inventory
		//cin.ignore(255, '\n');	//this doesn't work right now
		if (p1.getHealth() > 0)
		{
			cout << "\nI- go inside your inventory!\n";
			std::this_thread::sleep_for(std::chrono::milliseconds(1500));
			if (GetAsyncKeyState(0x49))	//TODO change this key to 'I'
			{
				cin.ignore(255, '\n');
				cin.clear();
				//clear buffer so that it doesn't go inside the if statement if the user spams iiiii to get into it once
				playerSkipTurn = p1.showAndIsUseInvCFight();
			}
		}//if player is alive
	} while (p1.getIsAlive() && boss.getIsAlive());	//while p1 is alive and dragon is alive, if one dies, fight is over
	return;
}

//Player steps into room and has their inventory filled with random treasure items
void Dungeon::treasureRoom(Player &p1, int currentRoomNumber)
{
	PlaySound(MAKEINTRESOURCE(IDR_WAVE15), NULL, SND_RESOURCE | SND_ASYNC);//IDR_WAVE15 is the treasureRoom
	cout << "You found the treasure room!\n";
	std::this_thread::sleep_for(std::chrono::seconds(1));
	//create an array of treasures
	int treasuresGold[] = { 25, 50, 100, 250, 500, 1000};	//6 items
	std::string treasureStatBoost[] = {"+1 Attack", "+3 Attack", "+5 Attack", "+1 Strength", "+3 Strength", "+5 Strength", "+1 Defence", "+3 Defence", "+5 Defence" };	//9 items
	std::string treasureFood[] = {"Fish", "Minion Meat", "Small Dragon Meat", "Shark", "Dragon Meat"};	//5 items
	//fish is +5 health, Minion meat is +10,Small Dragon Meat is +15, Shark is +20, Dragon meat is +25
	//randomly choose an array of treasure 5 times, for each drop table landed on, only pick 1 item randomly from it and add it to the player's inventory
     int amountOfRolls = (currentRoomNumber / 10) + 2;
	for (int i = 0; i < amountOfRolls; i++)	//from level 1-10, 2 roll, 11-20 will be 3 rolls, maxes out at 5 rolls
	{
		int randomNum = rand() % 3;
		if (randomNum == 0)	//land in the gold drop table
		{
			randomNum = rand() % 6;  //try to change this is the drop table.size() if possible instead of hardcoded number
			int gold = treasuresGold[randomNum];
			cout << "You recieved " << gold << " gold!\n";
			p1.setGold(p1.getGold() + gold);
		}
		else if (randomNum == 1)	//land in the stat boost drop table
		{
			randomNum = rand() % 9;
			p1.increaseStat(treasureStatBoost[randomNum]);
			
		}
		else if (randomNum == 2)	//land in the food drop table
		{
			randomNum = rand() % 5;
			p1.addItem(treasureFood[randomNum]);
		}
		else
		{
			cout << "Error generating correct number to land on drop table!\n";
		}
	}
}

//Player steps into room and is prompted with a shop, player can buy items if they have enough gold and can buy as many items as they want
void Dungeon::shopRoom(Player &p1)
{
	PlaySound(MAKEINTRESOURCE(IDR_WAVE16), NULL, SND_RESOURCE | SND_ASYNC);//IDR_WAVE16 is the shopRoom
	cout << "You found the shop room!A merchant has been awaiting for your arrival\n";
	std::this_thread::sleep_for(std::chrono::seconds(1));
	bool exit = false;
	int choice;
	do
	{
		cout << "Merchant's Store:\t\tYour gold: "<< p1.getGold() << "\n"
			<< "1. Fish (20 gold)\n"
			<< "2. Shark (100 gold)\n"
			<< "3. +1 Attack (200 gold)\n"
			<< "4. +1 Strength (200 gold)\n"
			<< "5. +1 Defence (200 gold)\n"
			<< "6. Full Heal (120 gold)\n"
			<< "0. Exit shop\n";
		cin >> choice;
		if (cin.fail())	//if it fails, it's not an int
		{
			cout << "That wasn't an int! Try typing a number to choose what to buy\n";
			cin.clear();
			cin.ignore(255, '\n');
			exit = false;
		}
		else if (choice < 0 || choice > 6)
		{
			cout << "That option is out of range!\n";
		}
		else	//else it's a valid option
		{
			switch (choice)
			{
			case 1:	//purchase Fish
				if (p1.getGold() >= 20)
				{
					p1.setGold(p1.getGold() - 20);
					p1.addItem("Fish");
				}
				else	//not enough gold
				{
					cout << "Not enough gold to purchase this item!\n";
				}
				break;
			case 2:	//purchase Shark
				if (p1.getGold() >= 100)
				{
					p1.setGold(p1.getGold() - 100);
					p1.addItem("Shark");
				}
				else	//not enough gold
				{
					cout << "Not enough gold to purchase this item!\n";
				}
				break;
			case 3:	//+1 attack
				if (p1.getGold() >= 200 && p1.getAttackStat() < 35)
				{
					p1.setGold(p1.getGold() - 200);
					p1.increaseStat("+1 Attack");
				}
				else	//not enough gold
				{
					if (p1.getAttackStat() >= 35)
					{
						cout << "Your attack stat is already maxed out!\n";
					}
					else
					{
						cout << "Not enough gold to purchase this item!\n";
					}
				}
				break;
			case 4:	//+1 str
				if (p1.getGold() >= 200 && p1.getStrengthStat() < 35)
				{
					p1.setGold(p1.getGold() - 200);
					p1.increaseStat("+1 Strength");
				}
				else	//not enough gold
				{
					if (p1.getStrengthStat() >= 35)
					{
						cout << "Your strength stat is already maxed out!\n";
					}
					else
					{
						cout << "Not enough gold to purchase this item!\n";
					}
				}
				break;
			case 5:	//+1 def
				if (p1.getGold() >= 200 && p1.getDefenceStat() < 35)
				{
					p1.setGold(p1.getGold() - 200);
					p1.increaseStat("+1 Defence");
				}
				else	//not enough gold
				{
					if (p1.getDefenceStat() >= 35)
					{
						cout << "Your defence stat is already maxed out!\n";
					}
					else
					{
						cout << "Not enough gold to purchase this item!\n";
					}
				}
				break;
			case 6:	//full heal
				if (p1.getGold() >= 120 && p1.getHealth() < 100)
				{
					p1.setGold(p1.getGold() - 120);
					p1.setHealth(100);
					cout << "Your health is now at: " << p1.getHealth() << "/100\n";
				}
				else	//not enough gold
				{
					if (p1.getHealth() >= 100)
					{
						cout << "You're already at max health!\n";
					}
					else
					{
						cout << "Not enough gold to purchase this item!\n";
					}
				}
				break;
			case 0:	//exit
				exit = true;
				break;
			default:
				cout << "Try again\n";
				break;
			}
		}
	} while (exit == false);

	cout << "Thanks for visiting!\n";
	return;
}

void Dungeon::swarmMinionRoom(Player & p1)
{
	//Player will always go first
	Minion minion1, minion2, minion3;	//minions spawned in the room
	bool playerSkipTurn = false;
	int userChoice;
	cout << "You've encountered a swarm of minion!\n";
	cout << "\t`oo.'\t\t\t`oo.'\t\t\t`oo.'\n"
		<< "\t`-')  ,.\t\t`-')  ,.\t\t`-')  ,.\n"
		<< "\t( `-'/^`\t\t( `-'/^`\t\t( `-'/^`\n"
		<< "\t-`J-d   \t\t-`J-d   \t\t-`J-d   \n";
	PlaySound(MAKEINTRESOURCE(IDR_WAVE5), NULL, SND_RESOURCE | SND_SYNC);//IDR_WAVE5 is the minion encounter
	PlaySound(MAKEINTRESOURCE(IDR_WAVE5), NULL, SND_RESOURCE | SND_SYNC);//IDR_WAVE5 is the minion encounter
	PlaySound(MAKEINTRESOURCE(IDR_WAVE5), NULL, SND_RESOURCE | SND_ASYNC);//IDR_WAVE5 is the minion encounter
	cout << "\nWould you like to try to fight or run?\n"
		<< "1)Fight\n"
		<< "2)Run\n";
	//check that userChoice is valid
	userChoice = intOneorTwo();	//this forces the user to type in a 1 or a 2 so I know userChoice is either a 1 or a 2 after that function
	if (userChoice == 2)
	{
		//attempt to escape
		bool hasEscaped = false;
		hasEscaped = minion1.isEscape();	//escape chance will still be 25%
		if (hasEscaped == true)
		{
			return;
		}
	}
	//else we fight
	system("CLS");	//clears the screen
	cout << "Player HP: " << p1.getHealth() << "\t\t\tMinion's HP: " << minion1.getHealth() << "\t" << minion2.getHealth() << "\t" << minion3.getHealth() << std::endl;
	do
	{
		if (playerSkipTurn == false)
		{
			PlaySound(MAKEINTRESOURCE(IDR_WAVE6), NULL, SND_RESOURCE | SND_SYNC);//IDR_WAVE6 is the playerHit
			if (minion1.getIsAlive() == true)	//attack the first minion if it's alive
			{
				minion1.defend(p1.attack());	//player attacked the minion and updated health
				if (minion1.getIsAlive() == false)	//if the minion died
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(1500));	//needed for player to see what they hit on the minion to kill it
					//since minion is dead, we prevent it's health from going negative
					minion1.setHealth(0);
					system("CLS");
					cout << "Player HP: " << p1.getHealth() << "\t\t\tMinion's HP: " << minion1.getHealth() << "\t" << minion2.getHealth() << "\t" << minion3.getHealth() << std::endl;
					cout << "You defeated the first minion!\n";
					minion1.deathLoot(p1);
				}
			}
			else if (minion2.getIsAlive() == true)	//if we're here, the first minion is dead and we attack the second
			{
				minion2.defend(p1.attack());
				if (minion2.getIsAlive() == false)	//if the minion died
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(1500));	//needed for player to see what they hit on the minion to kill it
					//since minion is dead, we prevent it's health from going negative
					minion2.setHealth(0);
					system("CLS");
					cout << "Player HP: " << p1.getHealth() << "\t\t\tMinion's HP: " << minion1.getHealth() << "\t" << minion2.getHealth() << "\t" << minion3.getHealth() << std::endl;
					cout << "You defeated the second minion!\n";
					minion2.deathLoot(p1);
				}
			}
			else	//else only the third minion is alive
			{
				minion3.defend(p1.attack());
				if (minion3.getIsAlive() == false)	//if the minion died
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(1500));	//needed for player to see what they hit on the minion to kill it
					//since minion is dead, we prevent it's health from going negative
					minion3.setHealth(0);
					system("CLS");
					cout << "Player HP: " << p1.getHealth() << "\t\t\tMinion's HP: " << minion1.getHealth() << "\t" << minion2.getHealth() << "\t" << minion3.getHealth() << std::endl;
					cout << "You defeated the swarm of minions!\n";
					minion3.deathLoot(p1);
					break;
				}
			}
		}
		else	//else it's true and the player skips a turn
		{
			cout << "\nYou just ate and can't hit this turn!\n";
			playerSkipTurn = false;
			std::this_thread::sleep_for(std::chrono::milliseconds(1500));
		}

		//End of turn for player attacking minion
		std::this_thread::sleep_for(std::chrono::milliseconds(1500));
		system("CLS");
		cout << "Player HP: " << p1.getHealth() << "\t\t\tMinion's HP: " << minion1.getHealth() << "\t" << minion2.getHealth() << "\t" << minion3.getHealth() << std::endl;
		if (minion1.getIsAlive() == true && p1.getIsAlive() == true)
		{
			PlaySound(MAKEINTRESOURCE(IDR_WAVE8), NULL, SND_RESOURCE | SND_SYNC);//IDR_WAVE8 is the minionHit, this one is sync because if it was async
																				 //the player is hit sound will play almost instantly and you won't hear minion hit sound
			p1.defend(minion1.attack());	//minion attacks the player
			PlaySound(MAKEINTRESOURCE(IDR_WAVE9), NULL, SND_RESOURCE | SND_SYNC);//IDR_WAVE9 is the playerIsHit
		}
			
		if (minion2.getIsAlive() == true && p1.getIsAlive() == true)
		{
			PlaySound(MAKEINTRESOURCE(IDR_WAVE8), NULL, SND_RESOURCE | SND_SYNC);//IDR_WAVE8 is the minionHit, this one is sync because if it was async
																				 //the player is hit sound will play almost instantly and you won't hear minion hit sound
			p1.defend(minion2.attack());	//minion attacks the player
			PlaySound(MAKEINTRESOURCE(IDR_WAVE9), NULL, SND_RESOURCE | SND_SYNC);//IDR_WAVE9 is the playerIsHit
		}
			
		if (minion3.getIsAlive() == true && p1.getIsAlive() == true)
		{
			PlaySound(MAKEINTRESOURCE(IDR_WAVE8), NULL, SND_RESOURCE | SND_SYNC);//IDR_WAVE8 is the minionHit, this one is sync because if it was async
																				 //the player is hit sound will play almost instantly and you won't hear minion hit sound
			p1.defend(minion3.attack());	//minion attacks the player
			PlaySound(MAKEINTRESOURCE(IDR_WAVE9), NULL, SND_RESOURCE | SND_SYNC);//IDR_WAVE9 is the playerIsHit
		}
		//std::this_thread::sleep_for(std::chrono::milliseconds(1500));
		system("CLS");
		cout << "Player HP: " << p1.getHealth() << "\t\t\tMinion's HP: " << minion1.getHealth() << "\t" << minion2.getHealth() << "\t" << minion3.getHealth() << std::endl;
		//cin.clear();	//clears buffer before player is prompted if they want to go in their inventory
		//cin.ignore(255, '\n');	//this doesn't work right now
		if (p1.getHealth() > 0)
		{
			cout << "\nI- go inside your inventory!\n";
			std::this_thread::sleep_for(std::chrono::milliseconds(1500));
			if (GetAsyncKeyState(0x49))	//TODO change this key to 'I'
			{
				cin.ignore(255, '\n');
				cin.clear();
				//clear buffer so that it doesn't go inside the if statement if the user spams iiiii to get into it once
				playerSkipTurn = p1.showAndIsUseInvCFight();
			}
		}//if player is alive

	} while (p1.getIsAlive());	//while p1 is alive, if all the minions die, we break out of the loop
	return;
}

//Function that deals with the combat in that room
void Dungeon::smallDragonRoom(Player & p1)
{
	SmallDragon drag;	//small dragon spawned in the room
	bool playerSkipTurn = false;
	int userChoice;
	cout << "You've encountered a small dragon!\n";
	//PlaySound(MAKEINTRESOURCE(IDR_WAVE10), NULL, SND_RESOURCE | SND_ASYNC);//IDR_WAVE10 is the dragon encounter
	cout << "\t<>=======() \n"
		<< "\t(/\\___   /|\\\\          ()==========<>_\n"
		<< "\t      \\_/ | \\\\        //|\\   ______/ \\)\n"
		<< "\t        \\_|  \\\\      // | \\_/\n"
		<< "\t          \\|\\/|\\_   //  /\\/\n"
		<< "\t           (oo)\\ \\_//  /\n"
		<< "\t          //_/\\_\\/ /  |\n"
		<< "\t         @@/  |=\\  \\  |\n"
		<< "\t              \\_=\\_ \\ |\n"
		<< "\t                \\==\\ \\|\\_ \n"
		<< "\t             __(\\===\\(  )\\\n"
		<< "\t            (((~) __(_/   |\n"
		<< "\t                 (((~) \\  /\n"
		<< "\t                 ______/ /\n"
		<< "\t                 '------'\n";


	cout << "\nWould you like to try to fight or run?\n"
		<< "1)Fight\n"
		<< "2)Run\n";
	//check that userChoice is valid
	userChoice = intOneorTwo();	//this forces the user to type in a 1 or a 2 so I know userChoice is either a 1 or a 2 after that function
	if (userChoice == 2)
	{
		//attempt to escape
		bool hasEscaped = false;
		hasEscaped = drag.isEscape();
		if (hasEscaped == true)
		{
			return;
		}
	}
	//else we fight
	system("CLS");	//clears the screen
	cout << "Player HP: " << p1.getHealth() << "\t\t\tSmall Dragon HP: " << drag.getHealth() << std::endl;
	do
	{
		if (playerSkipTurn == false)
		{
			PlaySound(MAKEINTRESOURCE(IDR_WAVE6), NULL, SND_RESOURCE | SND_SYNC);//IDR_WAVE6 is the playerHit, sync so that dragonBeingHit is heard after
			drag.defend(p1.attack());	//player attacked the dragon and updated health
			//PlaySound(MAKEINTRESOURCE(IDR_WAVE11), NULL, SND_RESOURCE | SND_SYNC);//IDR_WAVE10 is the dragonBeingHit
		}
		else	//else it's true and the player skips a turn
		{
			cout << "\nYou just ate and can't hit this turn!\n";
			playerSkipTurn = false;
			std::this_thread::sleep_for(std::chrono::milliseconds(1500));
		}
		if (drag.getIsAlive() == false)	//if dragon is dead
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1500));	//needed so player sees what they hit to kill the dragon
			drag.setHealth(0);
			system("CLS");
			cout << "Player HP: " << p1.getHealth() << "\t\t\tSmall Dragon HP: " << drag.getHealth() << std::endl;
			cout << "You defeated the small dragon!\n";
			drag.deathLoot(p1);
			break;
		}

		//End of turn for player attacking dragon
		std::this_thread::sleep_for(std::chrono::milliseconds(1500));
		system("CLS");
		cout << "Player HP: " << p1.getHealth() << "\t\t\tSmall Dragon HP: " << drag.getHealth() << std::endl;
		//PlaySound(MAKEINTRESOURCE(IDR_WAVE13), NULL, SND_RESOURCE | SND_SYNC);//IDR_WAVE13 is the dragonHit, sync to hear it before playerIsHit
		p1.defend(drag.attack());	//dragon attacks player
		PlaySound(MAKEINTRESOURCE(IDR_WAVE9), NULL, SND_RESOURCE | SND_SYNC);//IDR_WAVE9 is the playerIsHit
																			 //std::this_thread::sleep_for(std::chrono::milliseconds(1500));
		system("CLS");
		cout << "Player HP: " << p1.getHealth() << "\t\t\tSmall Dragon HP: " << drag.getHealth() << std::endl;
		//cin.clear();	//clears buffer before player is prompted if they want to go in their inventory
		//cin.ignore(255, '\n');	//this doesn't work right now
		if (p1.getHealth() > 0)
		{
			cout << "\nI- go inside your inventory!\n";
			std::this_thread::sleep_for(std::chrono::milliseconds(1500));
			if (GetAsyncKeyState(0x49))	//TODO change this key to 'I'
			{
				cin.ignore(255, '\n');
				cin.clear();
				//clear buffer so that it doesn't go inside the if statement if the user spams iiiii to get into it once
				playerSkipTurn = p1.showAndIsUseInvCFight();
			}
		}//if player is alive
	} while (p1.getIsAlive() && drag.getIsAlive());	//while p1 is alive and dragon is alive, if one dies, fight is over
	return;
}

Player::Player()
{
	health = 100;
	isAlive = true;
	gold = 0;
	attackStat = 15;	//base 15 stats
	strengthStat = 15;
	defenceStat = 15;
}

int Player::attack()
{
	//Player's attack will vary only based on the stats the player has
	//TODO implement stats to affect player attacks
	//attack will be accuracy
	//strength is max hit

	//for accuracy, if the roll is less than 20% of our max hit, we have a (attackStat)% chance of re-rolling for another hit
	int damage = (rand() % strengthStat);
	double rerollDmg = strengthStat * 0.20;	//this is 20% of our max hit
	if (damage < rerollDmg)
	{
		int chance = (rand() % 100);
		if (chance >= 0 && chance <= attackStat)	//this range is attackStat%
		{
			damage = (rand() % strengthStat);	//this still has the potential to be a weak or even worse hit but that's okay
			//cout << "You deal: " << damage << " damage!\n";
			return damage;
		}
		else	//else we don't reroll
		{
			//cout << "You deal: " << damage << " damage!\n";
			return damage;
		}
	}
	else	//else the damage is high enough to not need re-rolling
	{
		//cout << "You deal: " << damage << " damage!\n";
		return damage;
	}
}

//Player reacts to damage based off its defence stats
void Player::defend(int damage)
{
	//Player defence will have a chance to reduce the incoming attack by a percentage
	int ogDamage = damage;
	int chanceToReduce = (rand() % 100);
	if (chanceToReduce >= 0 && chanceToReduce <= defenceStat)	//The higher the defence stat, the higher range we cover to be able to reduce
	{
		//reduce the damage by 1% of the player's defence stat
		double reducedDmg = defenceStat * 0.1;
		damage -= reducedDmg;
		if (damage < 0)
			damage = 0;
		//now update the player's health
		cout << "You take: " << damage << " damage!(reduced by " << (int)reducedDmg+1 << ") from " << ogDamage << "\n";//the +1 is needed since it's rounded up
		health -= damage;	//updated player health
		if (health <= 0)
		{
			isAlive = false;
		}
		return;
	}
	else	//else we don't reduce the damage
	{
		cout << "You take: " << damage << " damage!\n";
		health -= damage;
		if (health <= 0)
		{
			isAlive = false;
		}
		return;
	}
}

//Adds an item to the player's inventory
void Player::addItem(std::string item)
{
	//TODO look into creating stack so objects aka if there is a shark in the player's iventory, make it display 2x sharks into of shark shark
	cout << "You recieved: " << item << "!\n";
	inventoryConsume.push_back(item);
}

//Takes in a boost and applies it to the player
void Player::increaseStat(std::string stat)
{
	//http://www.cplusplus.com/reference/string/string/find/
	std::string attackStr = "Attack";
	std::string strengthStr = "Strength";
	std::string defenceStr = "Defence";

	cout << "Your recieved a stat buff: " << stat << "!\n";

	std::size_t found = stat.find(attackStr);
	if (found != std::string::npos)	//if this is true, attack has been found and we apply the buff to attack
	{
		//in the string, the 1 spot is going to hold the number
		char number = stat[1];
		int buff = number - '0';	//this is the ascii value - 48 since the number 0 is 48 on ascii table
		if (attackStat >= 35)	//this is the cap so that the player cannot go above 35 in their stats
		{
			cout << "Your current attack stat is already maxed out at: " << attackStat << std::endl;
			return;
		}
		else
		{
			attackStat += buff;	//buff has been applied
			if (attackStat > 35)
				attackStat = 35;	//this way it cannot go above 35
			return;
		}
	}
	found = stat.find(strengthStr);
	if (found != std::string::npos)	//if this is true, strength has been found and we apply the buff to attack
	{
		//in the string, the 1 spot is going to hold the number
		char number = stat[1];
		int buff = number - '0';	//this is the ascii value - 48 since the number 0 is 48 on ascii table
		if (strengthStat >= 35)	//this is the cap so that the player cannot go above 35 in their stats
		{
			cout << "Your current strength stat is already maxed out at: " << strengthStat << std::endl;
			return;
		}
		else
		{
			strengthStat += buff;	//buff has been applied
			if (strengthStat > 35)
				strengthStat = 35;	//this way it cannot go above 35
			return;
		}
	}
	found = stat.find(defenceStr);
	if (found != std::string::npos)	//if this is true, strength has been found and we apply the buff to attack
	{
		//in the string, the 1 spot is going to hold the number
		char number = stat[1];
		int buff = number - '0';	//this is the ascii value - 48 since the number 0 is 48 on ascii table
		if (defenceStat >= 35)	//this is the cap so that the player cannot go above 35 in their stats
		{
			cout << "Your current defence stat is already maxed out at: " << defenceStat << std::endl;
			return;
		}
		else
		{
			defenceStat += buff;	//buff has been applied
			if (defenceStat > 35)
				defenceStat = 35;	//this way it cannot go above 35
			return;
		}
	}
	return;
}

//Displays player current health,attack,strength,defence stats
void Player::displayStat()
{
	cout << "\nHealth: " << health
		<< "\nAttack: " << attackStat
		<< "\nStrength: " << strengthStat
		<< "\nDefence: " << defenceStat << "\n";
}

//Shows the player the inventory and allows the player to use it
void Player::showAndUseInvC()
{
	bool exit = false;
	do
	{
		cout << "Gold: " << gold << "\n";
		for (int i = 0; i < inventoryConsume.size(); i++)
		{
			cout << (i + 1) << ". " << inventoryConsume[i] << std::endl;
		}
		cout << "0. to exit" << std::endl;
		bool valid = false;
		int answer;
		do
		{
			cin >> answer;
			if (answer < 0 || answer > inventoryConsume.size())
			{
				cout << "Invalid option!\n";
				cin.clear();
				cin.ignore(255, '\n');
				valid = false;
			}
			else
				valid = true;
		} while (valid == false);
		//once we're here, check if the answer was 0, if it is exit, if not, retrieve the item
		cout << "\n";
		if (answer == 0)
		{
			exit = true;
			return;
		}
		else
		{
			if (health >= 100)
			{
				cout << "You're health is already at 100 or above! You cannot eat anymore food!\n";
			}
			else
			{
				std::string item = inventoryConsume[answer - 1];
				//delete item from inventory and shift everything over
				//https://stackoverflow.com/questions/875103/how-do-i-erase-an-element-from-stdvector-by-index
				inventoryConsume.erase(inventoryConsume.begin() + (answer - 1));	//erase shifts everything over automatically
				useItem(item);
			}
		}//else if answer isn't 0
	} while (exit == false);
}

//Recieves an item to use on the player
void Player::useItem(std::string item)
{
	//"Fish", "Lobster", "Minion Meat", "Shark", "Dragon Meat"
	if (item == "Fish") { health += 5; }
	else if (item == "Minion Meat") { health += 10; }
	else if (item == "Small Dragon Meat") { health += 15; }
	else if (item == "Shark") { health += 20; }
	else if (item == "Dragon Meat") { health += 25; }
	cout << "You consumed the " << item << "! Current health is " << health << "/100\n";
	PlaySound(MAKEINTRESOURCE(IDR_WAVE18), NULL, SND_RESOURCE | SND_ASYNC);//IDR_WAVE18 is eating
	return;
}

bool Player::showAndIsUseInvCFight()
{
	for (int i = 0; i < inventoryConsume.size(); i++)
	{
		cout << (i + 1) << ". " << inventoryConsume[i] << std::endl;
	}
	cout << "0. to exit" << std::endl;
	bool valid = false;
	int answer;
	do
	{
		cin >> answer;
		if (answer < 0 || answer > inventoryConsume.size())
		{
			cout << "Invalid option!\n";
			cin.clear();
			cin.ignore(255, '\n');
			valid = false;
		}
		else
		valid = true;
	} while (valid == false);
	//once we're here, check if the answer was 0, if it is exit, if not, retrieve the item
	cout << "\n";
	if (answer == 0)
	{
		return false;
	}
	else
	{
		if (health >= 100)
		{
			cout << "You're health is already at 100 or above! You cannot eat anymore food!\n";
		}
		else
		{
			std::string item = inventoryConsume[answer - 1];
			//delete item from inventory and shift everything over
			//https://stackoverflow.com/questions/875103/how-do-i-erase-an-element-from-stdvector-by-index
			inventoryConsume.erase(inventoryConsume.begin() + (answer - 1));	//erase shifts everything over automatically
			useItem(item);
			return true;
		}
	}//else if answer isn't 0
return false;
}

Minion::Minion()
{
	setHealth(20);
	setAlive(true);
	setAttackStat(7);
	setStrengthStat(10);
	setDefenceStat(7);
}

int Minion::attack()
{
	//A minion should hit low and should never be able to kill the player unless the player was low to begin with
	//A minion should be a slight annoyance with potential to have ok loot to compensate for
	//TODO make a more rich attack

	int damage = (rand() % getStrengthStat());
	double rerollDmg = getStrengthStat() * 0.20;	//this is 20% of our max hit
	if (damage < rerollDmg)
	{
		int chance = (rand() % 100);
		if (chance >= 0 && chance <= getAttackStat())	//this range is attackStat%
		{
			damage = (rand() % getStrengthStat());	//this still has the potential to be a weak or even worse hit but that's okay
			//cout << "The minion deals: " << damage << " damage!\n";
			return damage;
		}
		else	//else we don't reroll
		{
			//cout << "The minion deals: " << damage << " damage!\n";
			return damage;
		}
	}
	else	//else the damage is high enough to not need re-rolling
	{
		//cout << "The minion deals: " << damage << " damage!\n";
		return damage;
	}
	
	return damage;	//minion can hit from 0-9
}

//Function returns true if the player sucefully escaped from the minion, 25% chance of escaping
bool Minion::isEscape()
{
	//player will have a 25% chance of escaping any minion encounter
	int chance = (rand() % 4);	//range is 0-3
	if (chance == 0)
	{
		cout << "You have succefully escaped from the minion!\n\n";
		return true;
	}
	cout << "The minion doesn't let you run away!\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(1200));	//the code that happens after this is a clear screen so this is needed to see the statement
	return false;	//else return false because the player did not manage to escape
}

//Function that tells the user the minion has died and rewards the player with loot
void Minion::deathLoot(Player & p1)
{
	PlaySound(MAKEINTRESOURCE(IDR_WAVE7), NULL, SND_RESOURCE | SND_SYNC);//IDR_WAVE7 is the minion death
	//TODO add better loot
	cout << "You recieved 50 gold!\n";
	p1.setGold(p1.getGold() + 50);
	std::string loot[] = { "Minion Meat" };	//expand on this later
	int lootChance = rand() % 2;	//0 or 1
	if (lootChance == 0)
	{
		p1.addItem("Minion Meat");
	}
	return;
}

Dragon::Dragon()
{
	setHealth(85);
	setAlive(true);
	setAttackStat(25);
	setStrengthStat(15);
	setDefenceStat(10);
}

//Dragon attack is just like any attack but has a 10% chance to burn the player and inflict high damage
int Dragon::attack()
{
	int damage = (rand() % getStrengthStat());
	double rerollDmg = getStrengthStat() * 0.20;	//this is 20% of our max hit
	int burnChance = (rand() % 10);
	if (burnChance == 0)	//burn the player
	{
		cout << "The Dragon burns you with his mighty dragon fire!\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(1200));
		return (damage + 20);
	}
	else if (damage < rerollDmg)	//else we don't burn the player and we check if we reroll the damage
	{
		int chance = (rand() % 100);
		if (chance >= 0 && chance <= getAttackStat())	//this range is attackStat%
		{
			damage = (rand() % getStrengthStat());	//this still has the potential to be a weak or even worse hit but that's okay
			return damage;
		}
		else	//else we don't reroll
		{
			return damage;
		}
	}
	else	//else the damage is high enough to not need re-rolling
	{
		return damage;
	}

	return damage;	//dragon can hit from 0-14
}

//Function return true if the player sucefully escaped from the Dragon, 5% chance of escaping
bool Dragon::isEscape()
{
	//player will have a 5% chance of escaping any minion encounter
	int chance = (rand() % 20);	//range is 0-19
	if (chance == 0)
	{
		cout << "You have succefully escaped from the Dragon!\n\n";
		return true;
	}
	cout << "The Dragon doesn't let you run away!\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(1200));	//the code that happens after this is a clear screen so this is needed to see the statement
	return false;	//else return false because the player did not manage to escape
}

//Function that tells the user the Dragon has died and rewards the player with loot
void Dragon::deathLoot(Player & p1)
{
	PlaySound(MAKEINTRESOURCE(IDR_WAVE12), NULL, SND_RESOURCE | SND_SYNC);//IDR_WAVE12 is the dragonDeath
	//TODO add loot
	cout << "You recieved 200 gold!\n";
	p1.setGold(p1.getGold() + 200);
	std::string loot[] = { "Dragon Meat" };	//expand on this later
	int lootChance = rand() % 2;	//0 or 1
	if (lootChance == 0)
	{
		p1.addItem("Dragon Meat");
	}
	return;
}

//Default mechanic for defending for all monsters unless the monster has special defensive mechanics
void Monster::defend(int damage)
{
	//Monster defence will have a chance to reduce the incoming attack by a percentage
	int ogDamage = damage;
	int chanceToReduce = (rand() % 100);
	if (chanceToReduce >= 0 && chanceToReduce <= defenceStat)	//The higher the defence stat, the higher range we cover to be able to reduce
	{
		//reduce the damage by 1% of the monster's defence stat
		double reducedDmg = defenceStat * 0.1;
		damage -= reducedDmg;
		if (damage < 0)
			damage = 0;
		//now update the monsters's health
		cout << "You deal: " << damage << " damage!(reduced by " << (int)reducedDmg+1 << ") from " << ogDamage << "\n";	//the +1 is needed since it's rounded up
		health -= damage;	//updated monsters health
		if (health <= 0)
		{
			isAlive = false;
		}
		return;
	}
	else	//else we don't reduce the damage
	{
		cout << "You deal: " << damage << " damage!\n";
		health -= damage;
		if (health <= 0)
		{
			isAlive = false;
		}
		return;
	}
}

//Default constructor for a small dragon
SmallDragon::SmallDragon()
{
	setHealth(35);
	setAlive(true);
	setAttackStat(12);
	setStrengthStat(12);
	setDefenceStat(7);
}

//Mechanic for a small dragon attack, include a weak dragon fire
int SmallDragon::attack()
{
	int damage = (rand() % getStrengthStat());
	double rerollDmg = getStrengthStat() * 0.20;	//this is 20% of our max hit
	int burnChance = (rand() % 10);
	if (burnChance == 0)	//burn the player
	{
		cout << "The small dragon burns you with its weak dragon fire!\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(1200));
		return (damage + 5);	//max hit is a 14
	}
	else if (damage < rerollDmg)	//else we don't burn the player and we check if we reroll the damage
	{
		int chance = (rand() % 100);
		if (chance >= 0 && chance <= getAttackStat())	//this range is attackStat%
		{
			damage = (rand() % getStrengthStat());	//this still has the potential to be a weak or even worse hit but that's okay
			return damage;
		}
		else	//else we don't reroll
		{
			return damage;
		}
	}
	else	//else the damage is high enough to not need re-rolling
	{
		return damage;
	}

	return damage;	//small dragon can hit from 0-11(14 with dragon fire)
}

//chance of escaping from the small dragon is 14%
bool SmallDragon::isEscape()
{
	//player will have a 14% chance of escaping any minion encounter
	int chance = (rand() % 7);	//range is 0-6
	if (chance == 0)
	{
		cout << "You have succefully escaped from the small dragon!\n\n";
		return true;
	}
	cout << "The small dragon doesn't let you run away!\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(1200));	//the code that happens after this is a clear screen so this is needed to see the statement
	return false;	//else return false because the player did not manage to escape
}

void SmallDragon::deathLoot(Player & p1)
{
	//PlaySound(MAKEINTRESOURCE(IDR_WAVE12), NULL, SND_RESOURCE | SND_SYNC);//IDR_WAVE12 is the dragonDeath
	//TODO add loot
	cout << "You recieved 100 gold!\n";
	p1.setGold(p1.getGold() + 100);
	std::string loot[] = { "Small Dragon Meat" };	//expand on this later
	int lootChance = rand() % 2;	//0 or 1
	if (lootChance == 0)
	{
		p1.addItem("Small Dragon Meat");
	}
	return;
}
