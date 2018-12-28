#pragma once
#ifndef GAMES_H
#define GAMES_H

const int amountOfEncounterTypes = 5;


enum class EGameStatus
{
	Invalid_guess,
	Play,
	EndGame,
	Move_forward
};

enum class ETypeOfEncounter
{
	Treasure,
	Minion,
	Empty,
	Boss,
	Shop
};


class Player
{
private:
	int health;
	bool isAlive;
public:
	Player();
	void setHealth(int _health) { health = _health; }
	void setAlive(bool _isAlive) { isAlive = _isAlive; }

	int getHealth() { return health; }
	bool getIsAlive() { return isAlive; }
	
	int attack();
};


class Monster
{
private:
	int health;
	bool isAlive;
public:
	Monster() { ; }
	void setHealth(int _health) { health = _health; }
	void setAlive(bool _isAlive) { isAlive = _isAlive; }

	int getHealth() { return health; }
	bool getIsAlive() { return isAlive; }

	virtual int attack() = 0;
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


class Dungeon
{
private:
	ETypeOfEncounter roomType;
public:
	Dungeon();
	void setRoomType(ETypeOfEncounter _roomType) { roomType = _roomType; }

	ETypeOfEncounter getRoomType() { return roomType; }

	void generateRoomType();
	void emptyRoom();
	void minionRoom(Player &p1);
};


class Game
{
private:
	bool gameOver;
	Dungeon dungeonRoom;
	int maxRooms;	//keeps track of how many rooms there is supposed to be during the game
	int currentRoomNumber;
	Player player;
public:
	Game();
	void setGameOver(bool _gameOver) { gameOver = _gameOver; }
	void setMaxRooms(int _maxRooms) { maxRooms = _maxRooms; }
	void setCurrentRoomNumber(int _currentRoomNumber) { currentRoomNumber = _currentRoomNumber; }

	bool getGameOver() { return gameOver; }
	int getMaxRooms() { return maxRooms; }
	int getCurrentRoomNumber() { return currentRoomNumber; }

	EGameStatus playGame();
};




#endif // !GAMES_H

