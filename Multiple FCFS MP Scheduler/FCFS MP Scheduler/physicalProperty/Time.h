#pragma once

#include "..\globals\Globals.h"

#include <string>
#include <iostream>

using namespace std;

class Time
{
public:

	static const string SEC;
	static const string MIN;
	static const string HOUR;

	static const int HOUR2SEC;
	static const int MIN2SEC;

	static const Time ZERO;
	static const Time NaN;

	static const Time TIMEMIN;
	static const Time TIMEMAX;


	/* 0.1 seconds */
	static const string PSEC;

	static const int HOUR2PSEC;
	static const int MIN2PSEC;
	static const int SEC2PSEC;


private:
	
	int time;

public:

	Time();
	Time(int time, string unit);

	int		get(string unit) const;
	Time	sum(Time newTime);
	Time	subtract(Time newTime);
	Time	proportion(int percentage);
};

