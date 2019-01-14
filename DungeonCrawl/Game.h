#pragma once
#ifndef GAMES_H
#define GAMES_H

#include "Items.h"
#include <vector>
#include <string>

const int amountOfEncounterTypes = 6;


enum class EGameStatus
{
	Invalid_guess,
	Play,
	EndGame,
	Move_forward,
	User_won
};

enum class ETypeOfEncounter
{
	Null,
	Treasure,
	Minion,
	Empty,
	Boss,
	Shop,
	SwarmOfMinions
};


class Player
{
private:
	int health;
	bool isAlive;
	int gold;
	int attackStat;
	int strengthStat;
	int defenceStat;
	std::vector<std::string> inventory;
public:
	Player();
	void setHealth(int _health) { health = _health; }
	void setAlive(bool _isAlive) { isAlive = _isAlive; }\
	void setGold(int _gold) { gold = _gold; }
	void setAttackStat(int _attackStat) { attackStat = _attackStat; }
	void setStrengthStat(int _strengthStat) { strengthStat = _strengthStat; }
	void setDefenceStat(int _defenceStat) { defenceStat = _defenceStat; }

	int getHealth() { return health; }
	bool getIsAlive() { return isAlive; }
	int getGold() { return gold; }
	int getAttackStat() { return attackStat; }
	int getStrengthStat() { return strengthStat; }
	int getDefenceStat() { return defenceStat; }
	
	int attack();
	void defend(int damage);
	void addItem(std::string);
	void increaseStat(std::string);
	void displayStat();
	void showAndUseInv();
	void useItem(std::string);
	bool showAndIsUseInvFight();
};


class Monster
{
private:
	int health;
	bool isAlive;
	int attackStat;
	int strengthStat;
	int defenceStat;
public:
	Monster() { ; }
	void setHealth(int _health) { health = _health; }
	void setAlive(bool _isAlive) { isAlive = _isAlive; }
	void setAttackStat(int _attackStat) { attackStat = _attackStat; }
	void setStrengthStat(int _strengthStat) { strengthStat = _strengthStat; }
	void setDefenceStat(int _defenceStat) { defenceStat = _defenceStat; }

	int getHealth() { return health; }
	bool getIsAlive() { return isAlive; }
	int getAttackStat() { return attackStat; }
	int getStrengthStat() { return strengthStat; }
	int getDefenceStat() { return defenceStat; }

	virtual int attack() = 0;
	void defend(int damage);
	virtual bool isEscape() = 0;
};


class Minion : public Monster
{
private:

public:
	Minion();
	int attack();
	bool isEscape();
};


class Boss : public Monster
{
private:

public:
	Boss();
	int attack();
	bool isEscape();
};

class Dungeon
{
private:
	ETypeOfEncounter roomType;
public:
	Dungeon();
	void setRoomType(ETypeOfEncounter _roomType) { roomType = _roomType; }

	ETypeOfEncounter getRoomType() { return roomType; }

	void generateRoomType(ETypeOfEncounter &);
	void emptyRoom();
	void minionRoom(Player &p1);
	void bossRoom(Player &p1);
	void treasureRoom(Player &p1);
	void shopRoom(Player &p1);
	void swarmMinionRoom(Player &p1);
};


class Game
{
private:
	bool gameOver;
	Dungeon dungeonRoom;
	int maxRooms;	//keeps track of how many rooms there is supposed to be during the game
	int currentRoomNumber;
	Player player;
	ETypeOfEncounter lastRoom;
public:
	Game();
	void setGameOver(bool _gameOver) { gameOver = _gameOver; }
	void setMaxRooms(int _maxRooms) { maxRooms = _maxRooms; }
	void setCurrentRoomNumber(int _currentRoomNumber) { currentRoomNumber = _currentRoomNumber; }

	bool getGameOver() { return gameOver; }
	int getMaxRooms() { return maxRooms; }
	int getCurrentRoomNumber() { return currentRoomNumber; }

	EGameStatus playGame();
	void showEndStats();
};




#endif // !GAMES_H

