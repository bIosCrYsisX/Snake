///////////////////////////////////////////////////////////////////////
// Workfile: Snake.h
// Author: Christoph Dalpiaz
// Date: 24.07.2020
// Description: snake game in console
// Remarks: -
// Revision: 0
///////////////////////////////////////////////////////////////////////

#ifndef SNAKE_H
#define SNAKE_H

#include "SnakeTypesConstants.h"

class Snake {
private:
	SnakeTC::Gamemap mGame;		//map of the game
	SnakeTC::TSnake mSnake;		//moving snake
	SnakeTC::Records mRecords;	//game records
	size_t mCount = 0;			//counter for actual points

	//initializes game map
	void Init();

	//returns true if snake hits wall or bites itself
	bool IsLosingPosition(size_t const posY, size_t const posX);

	//adds position to map and prints it, also resizes the snake and optionally makes new food
	//returns false if game is lost
	bool AddPosition(size_t const posY, size_t const posX);

	//changes position in dependence of key
	void AdaptPosition(int const key, size_t& posX, size_t& posY);

	//sets cursor for cout to new position
	void SetCursorPosition(size_t const y, size_t const x);

	//returns a point for the new food
	SnakeTC::TPoint GetFoodPoint();

	//reads records from RecordsFile
	void ReadRecords();

	//write records to file
	void WriteRecords();
public:
	//default constructor initializing game map
	Snake();

	//start the game
	void Play();
};

#endif