#pragma once
#ifndef GAMES_H
#define GAMES_H

class Dungeon;

enum class EGameStatus
{
	Invalid_guess,
	Play,
	EndGame
};

class Game
{
private:
	bool gameOver;
	Dungeon dungeonRoom;
	int maxRooms;	//keeps track of how many rooms there is supposed to be during the game
	int currentRoomNumber;
public:
	Game();
	void setGameOver(bool _gameOver) { gameOver = _gameOver; }
	void setMaxRooms(int _maxRooms) { maxRooms = _maxRooms; }
	void setCurrentRoomNumber(int _currentRoomNumber) { currentRoomNumber = _currentRoomNumber; }

	bool getGameOver() { return gameOver; }
	int getMaxRooms() { return maxRooms; }
	int getCurrentRoomNumber() { return currentRoomNumber; }

	void playGame();
};

class Dungeon
{
private:

public:
	Dungeon();
};

#endif // !GAMES_H

