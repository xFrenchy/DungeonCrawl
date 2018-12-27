#pragma once
#ifndef GAMES_H
#define GAMES_H


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

class Dungeon
{
private:
	ETypeOfEncounter roomType;
public:
	Dungeon();
};

class Player
{
private:
	int health;
public:
	Player();
	void setHealth(int _health) { health = _health; }

	int getHealth() { return health; }
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

