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
			dungeonRoom.bossRoom(player);
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
			dungeonRoom.treasureRoom(player);
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
	cout << "You've encountered a minion!\n";

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
		minion.defend(p1.attack());	//player attacked the minion and updated health
		if (minion.getIsAlive() == false)	//if the minion died
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(2400));
			system("CLS");
			cout << "Player HP: " << p1.getHealth() << "\t\t\tMinion HP: " << minion.getHealth() << std::endl;
			cout << "You defeated the minion!\n";
			//TODO add loot
			break;
		}

		//End of turn for player attacking minion
		std::this_thread::sleep_for(std::chrono::milliseconds(2400));
		system("CLS");
		cout << "Player HP: " << p1.getHealth() << "\t\t\tMinion HP: " << minion.getHealth() << std::endl;

		p1.defend(minion.attack());	//minion attacks the player
		std::this_thread::sleep_for(std::chrono::milliseconds(2400));
		system("CLS");
		cout << "Player HP: " << p1.getHealth() << "\t\t\tMinion HP: " << minion.getHealth() << std::endl;
	} while (p1.getIsAlive() && minion.getIsAlive());	//while p1 is alive and minion is alive, if one dies, fight is over
	return;
}

void Dungeon::bossRoom(Player & p1)
{
	Boss boss;	//boss spawned in the room
	int userChoice;
	cout << "You've encountered a boss!\n";

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
	cout << "Player HP: " << p1.getHealth() << "\t\t\tBoss HP: " << boss.getHealth() << std::endl;
	do
	{
		boss.defend(p1.attack());	//player attacked the boss and updated health
		if (boss.getIsAlive() == false)	//if boss is dead
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(2400));
			system("CLS");
			cout << "Player HP: " << p1.getHealth() << "\t\t\tBoss HP: " << boss.getHealth() << std::endl;
			cout << "You defeated the boss!\n";
			//TODO add loot
			break;
		}

		//End of turn for player attacking boss
		std::this_thread::sleep_for(std::chrono::milliseconds(2400));
		system("CLS");
		cout << "Player HP: " << p1.getHealth() << "\t\t\tBoss HP: " << boss.getHealth() << std::endl;

		p1.defend(boss.attack());	//boss attacks player
		std::this_thread::sleep_for(std::chrono::milliseconds(2400));
		system("CLS");
		cout << "Player HP: " << p1.getHealth() << "\t\t\tBoss HP: " << boss.getHealth() << std::endl;
	} while (p1.getIsAlive() && boss.getIsAlive());	//while p1 is alive and boss is alive, if one dies, fight is over
	return;
}

//Player steps into room and has their inventory filled with random treasure items
void Dungeon::treasureRoom(Player & p1)
{
	//create an array of treasures
	std::string treasuresGold[] = { "25 gold", "50 gold", "100 gold", "250 gold", "500 gold", "1000 gold" };	//6 items
	std::string treasureStatBoost[] = {"+1 Attack", "+3 Attack", "+5 Attack", "+1 Strength", "+3 Strength", "+5 Strength", "+1 Defence", "+3 Defence", "+5 Defence" };	//9 items
	std::string treasureFood[] = {"Fish", "Lobster", "Minion Meat", "Shark", "Dragon Meat"};	//5 items
	//fish is +5 health, lobster is +10, Minion meat is +15, Shark is +20, Dragon meat is +25
	//randomly choose an array of treasure 5 times, for each drop table landed on, only pick 1 item randomly from it and add it to the player's inventory
	for (int i = 0; i < 5; i++)
	{
		int randomNum = rand() % 3;
		if (randomNum == 0)	//land in the gold drop table
		{
			randomNum = rand() % 6;
			p1.addItem(treasuresGold[randomNum]);
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

Player::Player()
{
	health = 100;
	isAlive = true;
	attackStat = 15;
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
		//reduce the damage by 5% of the players defence stats
		double reducedDmg = damage * 0.05;
		damage -= reducedDmg;
		//now update the player's health
		cout << "You take: " << damage << " damage!(reduced) from " << ogDamage << "\n";
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
	inventory.push_back(item);
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
		attackStat += buff;	//buff has been applied
		return;
	}
	found = stat.find(strengthStr);
	if (found != std::string::npos)	//if this is true, strength has been found and we apply the buff to attack
	{
		//in the string, the 1 spot is going to hold the number
		char number = stat[1];
		int buff = number - '0';	//this is the ascii value - 48 since the number 0 is 48 on ascii table
		strengthStat += buff;	//buff has been applied
		return;
	}
	found = stat.find(defenceStr);
	if (found != std::string::npos)	//if this is true, strength has been found and we apply the buff to attack
	{
		//in the string, the 1 spot is going to hold the number
		char number = stat[1];
		int buff = number - '0';	//this is the ascii value - 48 since the number 0 is 48 on ascii table
		defenceStat += buff;	//buff has been applied
		return;
	}
	return;
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

/*void Minion::defend(int damage)
{
	//Minion defence will have a chance to reduce the incoming attack by a percentage
	int chanceToReduce = (rand() % 100);
	if (chanceToReduce >= 0 && chanceToReduce <= getDefenceStat())	//The higher the defence stat, the higher range we cover to be able to reduce
	{
		//reduce the damage by 5% of the minion defence stats
		double reducedDmg = damage * 0.05;
		damage -= reducedDmg;
		//now update the minion's health
		cout << "The minion takes: " << damage << "damage!(reduced)\n";
		setHealth(getHealth() - damage);	//updated minion health
		if (getHealth() <= 0)
		{
			setAlive(false);
		}
		return;
	}
	else	//else we don't reduce the damage
	{
		cout << "The minion takes: " << damage << "damage!\n";
		setHealth(getHealth() - damage);	//updated minion health
		if (getHealth() <= 0)
		{
			setAlive(false);
		}
		return;
	}
}*/

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

Boss::Boss()
{
	setHealth(85);
	setAlive(true);
	setAttackStat(25);
	setStrengthStat(15);
	setDefenceStat(5);
}

//Boss attack is just like any attack but has a 10% chance to burn the player and inflict high damage
int Boss::attack()
{
	int damage = (rand() % getStrengthStat());
	double rerollDmg = getStrengthStat() * 0.20;	//this is 20% of our max hit
	int burnChance = (rand() % 10);
	if (burnChance == 0)	//burn the player
	{
		cout << "The boss burns you with his mighty dragon fire!\n";
		//cout << "The boss deals: " << (damage + 20) << " damage!\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(1200));
		return (damage + 20);
	}
	else if (damage < rerollDmg)	//else we don't burn the player and we check if we reroll the damage
	{
		int chance = (rand() % 100);
		if (chance >= 0 && chance <= getAttackStat())	//this range is attackStat%
		{
			damage = (rand() % getStrengthStat());	//this still has the potential to be a weak or even worse hit but that's okay
			//cout << "The boss deals: " << damage << " damage!\n";
			return damage;
		}
		else	//else we don't reroll
		{
			//cout << "The boss deals: " << damage << " damage!\n";
			return damage;
		}
	}
	else	//else the damage is high enough to not need re-rolling
	{
		//cout << "The boss deals: " << damage << " damage!\n";
		return damage;
	}

	return damage;	//boss can hit from 0-14
}

//Function return true if the player sucefully escaped from the boss, 5% chance of escaping
bool Boss::isEscape()
{
	//player will have a 5% chance of escaping any minion encounter
	int chance = (rand() % 20);	//range is 0-19
	if (chance == 0)
	{
		cout << "You have succefully escaped from the boss!\n\n";
		return true;
	}
	cout << "The boss doesn't let you run away!\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(1200));	//the code that happens after this is a clear screen so this is needed to see the statement
	return false;	//else return false because the player did not manage to escape
}

void Monster::defend(int damage)
{
	//Monster defence will have a chance to reduce the incoming attack by a percentage
	int ogDamage = damage;
	int chanceToReduce = (rand() % 100);
	if (chanceToReduce >= 0 && chanceToReduce <= defenceStat)	//The higher the defence stat, the higher range we cover to be able to reduce
	{
		//reduce the damage by 5% of the monsters defence stats
		double reducedDmg = damage * 0.05;
		damage -= reducedDmg;
		//now update the monsters's health
		cout << "You deal: " << damage << " damage!(reduced) from " << ogDamage << "\n";
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
