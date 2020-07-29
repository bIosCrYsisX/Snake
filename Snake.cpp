///////////////////////////////////////////////////////////////////////
// Workfile: Snake.cpp
// Author: Christoph Dalpiaz
// Date: 24.07.2020
// Description: snake game in console
// Remarks: -
// Revision: 0
///////////////////////////////////////////////////////////////////////

#include <iostream>		//include IO library
#include <fstream>		//include filestream library
#include <algorithm>	//include standard library algorithms
#include <conio.h>		//needed for _getch()
#include <Windows.h>
#include <cstdlib>		//needed for random number generator
#include <time.h>		//needed for Sleep()
#include "Snake.h"		//include module header

using namespace std;		//resolve std namespace
using namespace SnakeTC;	//resolve namespace for types and constants

//overloaded << operator for Gamemap
ostream& operator<<(ostream& o, Gamemap const& game) {
	if (o.good()) {		//check if ostream is valid
		for_each(game.cbegin(), game.cend(), [&](std::array<char, NoColumns> const& line) { for_each(line.cbegin(), line.cend(), [&](char c) { o << c; }); o << endl;  });
	}
	return o;  //return manipulated ostream
}

//default constructor initializing game map
Snake::Snake() {
	Init();  //initialize the game map
}

void Snake::Play() {
	size_t posY = 0;  //variables for position
	size_t posX = 0;

	SetCursorPosition(CountDistanceY, CountDistanceX);		//set cout cursor to position for max score
	cout << MaxScoreText << (mRecords.size() > 0 ? *mRecords.crbegin() : 0);	//print maximum score from records file

	int key = _getch();  //get direction

	do {
		SetCursorPosition(CountDistanceY + MaxAndScoreDistance, CountDistanceX);  //set cout cursor to position for counter
		cout << PointsText << mCount;
		SetCursorPosition(NoLines + 1, 0);	//set cursor past the map
		Sleep(DelayMillis);		//wait a moment before moving snake
		if (_kbhit()) {			//if there is a keyboard hit, change direction
			key = _getch();
		}

		if (key == Pause) {  //check if user pressed pause
			SetCursorPosition(CountDistanceY + PauseYDistance, CountDistanceX);  //set cursor and write pause text
			cout << PauseText;
			while (key != MoveLeft && key != MoveRight && key != MoveDown && key != MoveUp) {   //wait nothing until user wants to move
				key = _getch(); 
			}
			SetCursorPosition(CountDistanceY + PauseYDistance, CountDistanceX); //clear pause text
			cout << EmptyLine;
			SetCursorPosition(NoLines + 1, 0);  //set cursor after game map
		}

		AdaptPosition(key, posX, posY);  //adapt position in dependece of key

	} while (AddPosition(posY, posX));  //add position to snake until game is lost

	//find insertion position for new record
	auto InsertPos = find_if(mRecords.cbegin(), mRecords.cend(), [&](size_t const val) { return val > mCount; });	

	if (InsertPos == mRecords.cend()) {  //if position is end of vecor, it is new record
		cout << RecordText << endl;
	}
	mRecords.insert(InsertPos, mCount);		//insert new record
	WriteRecords();							//write records to file

	cout << "You reached " << mCount << " points!" << endl;
	_getch();
}

//initializes game map
void Snake::Init() {
	for_each(mGame[0].begin(), mGame[0].end(), [](char& c) { c = WallChar; });				//fill first and last line with wall
	for_each(mGame[NoLines - 1].begin(), mGame[NoLines - 1].end(), [](char& c) {c = WallChar; });
	for_each(mGame.begin() + 1, mGame.end() - 1, [](std::array<char, NoColumns>& line) { line[0] = WallChar; line[NoColumns - 1] = WallChar;  //fill borders with wall
			 for_each(line.begin() + 1, line.end() - 1, [](char& c) {c = EmptyField; }); });												  //and the rest with empty characters

	mGame[StartPosY][StartPosX] = SnakeChar;			//draw snake begin
	mSnake.push_back(TPoint{ StartPosY, StartPosX });	//and save the position
	
	srand(time(nullptr));  //set seed for random generator

	TPoint foodPoint = GetFoodPoint();					//get a random point for snake food
	mGame[foodPoint.line][foodPoint.column] = FoodChar; //and save it on map

	ReadRecords();	//read records from RecordsFile

	cout << Introduction << endl << mGame;  //print map to console
}

//returns true if snake hits wall or bites itself
bool Snake::IsLosingPosition(size_t const posY, size_t const posX) {
	return mGame[posY][posX] == SnakeChar || mGame[posY][posX] == WallChar;  
}

//adds position to map and prints it, also resizes the snake and optionally makes new food
//returns false if game is lost
bool Snake::AddPosition(size_t const posY, size_t const posX) {
	if (posY > 0 && posX > 0 && posY < NoLines - 1 && posX < NoColumns - 1) {  //check if position is in bounds
		mSnake.push_back(TPoint{ posY, posX });  //add new position to snake

		if (mGame[posY][posX] != FoodChar) {	//no food hit, snake remains same size
			mGame[mSnake.cbegin()->line][mSnake.cbegin()->column] = EmptyField;				//delete snake end on map
			SetCursorPosition(mSnake.cbegin()->line + LineOffset, mSnake.cbegin()->column); //set cursor for cout to position
			cout << EmptyField;  //overwrite the snake end
			mSnake.pop_front();  //and delete it from snake

			if (mGame[posY][posX] == SnakeChar || mGame[posY][posX] == WallChar) {  //if snake hit a wall or itself
				SetCursorPosition(NoLines + 1, 0);  //set cursor after the map
				cout.flush();						

				return false;  //game is over
			}
		}
		else {  //if snake hits a food, it becomes bigger
			TPoint foodPoint = GetFoodPoint();					//get new point for food, add it to map and print it
			mGame[foodPoint.line][foodPoint.column] = FoodChar;
			SetCursorPosition(foodPoint.line + LineOffset, foodPoint.column);
			cout << FoodChar;

			mCount++;  //increment food counter
		}
		mGame[posY][posX] = SnakeChar;				//write snake character to new position
		SetCursorPosition(posY + LineOffset, posX);	//print the snake character
		cout << SnakeChar;
		SetCursorPosition(NoLines + 1, 0);

		cout.flush();

		return true;	//game continues
	}
	return false;	//game is over
}

//changes position in dependence of key
void Snake::AdaptPosition(int const key, size_t& posX, size_t& posY) {
	switch (key) {	//adapt position dependent on keypress
		case MoveLeft:  posY = mSnake.crbegin()->line;	   posX = mSnake.crbegin()->column - 1; break;
		case MoveRight: posY = mSnake.crbegin()->line;	   posX = mSnake.crbegin()->column + 1; break;
		case MoveDown:  posY = mSnake.crbegin()->line + 1; posX = mSnake.crbegin()->column;		break;
		case MoveUp:    posY = mSnake.crbegin()->line - 1; posX = mSnake.crbegin()->column;
	}
}

//sets cursor for cout to new position
void Snake::SetCursorPosition(size_t const y, size_t const x) {
	static HANDLE const hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	cout.flush();
	SetConsoleCursorPosition(hOut, COORD{ (short)x, (short)y });
}

//returns a point for the new food
TPoint Snake::GetFoodPoint() {
	TPoint foodPoint{ 0,0 };

	//find points until there is an open one
	while (mGame[foodPoint.line][foodPoint.column] != EmptyField) {
		foodPoint.line = rand() % (NoLines - 1) + 1;		//get random coordinates
		foodPoint.column = rand() % (NoColumns - 1) + 1;
	}

	return foodPoint;   //return the point
}

//reads records from RecordsFile
void Snake::ReadRecords() {
	ifstream inFile{ RecordsFile };  //define a filestream

	if (inFile.is_open()) {  //check if opening worked, if yes, copy all values to records vector
		copy(istream_iterator<size_t>{inFile}, istream_iterator<size_t>{}, back_inserter(mRecords));
		inFile.close();		//close filestream
	}
}

//write records to file
void Snake::WriteRecords() {
	ofstream writeFile{ RecordsFile };	//define a filestream

	if (writeFile.is_open()) {
		copy(mRecords.cbegin(), mRecords.cend(), ostream_iterator<size_t>{writeFile, "\n"});
		writeFile.close();	//close filestream
	}
}