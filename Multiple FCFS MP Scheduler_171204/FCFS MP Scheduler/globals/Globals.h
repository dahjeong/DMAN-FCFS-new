/*
This class has Global constants for first-come first-served scheduling algorithm.
*/

#pragma once
#include "..\physicalProperty\Time.h"
#include "..\physicalProperty\Length.h"




struct AircraftSequence {

	string	leadCategory;
	string	leadState;
	string	trailCategory;
	string	trailState;
	int		leadDirection;
	int		trailDirection;
};


class Globals
{
public:
	
	static const int MIN;
	static const int MAX;
	static const int NaN;


	Globals();
};

