///////////////////////////////////////////////////////////////////////
// Workfile: SnakeTypesConstants.h
// Author: Christoph Dalpiaz
// Date: 24.07.2020
// Description: types and constants for Snake module
// Remarks: -
// Revision: 0
///////////////////////////////////////////////////////////////////////

#ifndef SNAKETYPESCONSTANTS_H
#define SNAKETYPESCONSTANTS_H

#include <list>		//include std::list
#include <vector>	//include std::vector
#include <array>	//include std::array
#include <string>	//include std::string

namespace SnakeTC {		//namespace for types and constants

	std::string const Introduction = "Move with 4,5,6,8! Pause with Space";
	std::string const RecordsFile = "SnakeRecords.txt";
	std::string const PointsText = "Points: ";
	std::string const MaxScoreText = "Maximum score: ";
	std::string const RecordText = "This is a new record!";
	std::string const PauseText = "PAUSE";
	std::string const EmptyLine(PauseText.size(), ' ');

	//control keys for playing
	char const MoveUp = '8';
	char const MoveDown = '5';
	char const MoveLeft = '4';
	char const MoveRight = '6';
	char const Pause = 32;

	std::string const Seperator = ", ";

	//constraints for the map
	size_t const NoLines = 24;
	size_t const NoColumns = 34;

	//offset needed for printing changes on map
	size_t const LineOffset = 1;

	//time waited for refreshing
	size_t const DelayMillis = 125;

	//distance of counter to game map
	size_t const CountDistanceX = NoColumns + 15;
	size_t const CountDistanceY = NoLines / 2 + LineOffset;

	//distance from counter for pausetext
	size_t const PauseYDistance = 4;

	//lines between max score and actual points
	size_t const MaxAndScoreDistance = 2;

	//starting position of snake
	size_t const StartPosX = 15;
	size_t const StartPosY = 15;

	//characters representing objects on map
	char const WallChar = '#';
	char const EmptyField = ' ';
	char const SnakeChar = 'O';
	char const FoodChar = '*';

	struct TPoint {		//structure for a point on map
		size_t line;
		size_t column;
	};

	typedef std::array<std::array<char, NoColumns>, NoLines> Gamemap;  //type for game map
	typedef std::list<TPoint> TSnake;								   //type for the snake
	typedef std::vector<size_t> Records;							   //type for the records

};

#endif