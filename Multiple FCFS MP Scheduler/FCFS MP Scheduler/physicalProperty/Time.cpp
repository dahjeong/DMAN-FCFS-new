#include "Time.h"

// 최소 단위: 초

const string Time::SEC = "sec";
const string Time::MIN = "min";
const string Time::HOUR = "hour";

const int Time::HOUR2SEC = 3600;
const int Time::MIN2SEC = 60;

const Time Time::ZERO = Time(0, Time::SEC);
const Time Time::NaN = Time(-1, Time::SEC);


const Time Time::TIMEMIN = Time(Globals::MIN, Time::PSEC);
const Time Time::TIMEMAX = Time(Globals::MAX, Time::PSEC);


/* 0.1 second */
const string Time::PSEC = "p_sec";

const int Time::HOUR2PSEC = 3600*10;
const int Time::MIN2PSEC = 60*10;
const int Time::SEC2PSEC = 10;


Time::Time() :time(0) {}

Time::Time(int time, string unit) : time(time)
{
	if (unit == Time::PSEC) {

		this->time = time;

	} else if (unit == Time::SEC) {
		
		this->time = time*Time::SEC2PSEC;
	
	} else if (unit == Time::MIN) {

		this->time = time*Time::MIN2PSEC;

	} else if (unit == Time::HOUR) {

		this->time = time*Time::HOUR2PSEC;

	} else {

		cout << "Wrong input!" << endl;
		system("pause");
	}
}

int Time::get(string unit) const {

	if (unit == Time::PSEC) {

		return time;

	} else if (unit == Time::SEC) {

		return (int) (double)time/(double)Time::SEC2PSEC;

	} else if (unit == Time::MIN) {

		return (int)(double)time/(double)Time::MIN2PSEC;

	} else if (unit == Time::HOUR) {

		return (int)(double)time/(double)Time::HOUR2PSEC;

	} else {

		cout << "Wrong input!" << endl;
		system("pause");
		return -1;
	}
}

Time Time::sum(Time newTime) {

	return Time(time + newTime.get(Time::PSEC), Time::PSEC);
}

Time Time::subtract(Time newTime) {

	return Time(time - newTime.get(Time::PSEC), Time::PSEC);
}

Time Time::proportion(int percentage) {

	double r = (double) percentage/100;

	double newTime = (double) time*(1 + r);

	if (percentage >= 0) {
		newTime = floor(newTime);
	} else {
		newTime = ceil(newTime);
	}

	return Time((int) newTime, Time::PSEC);
}
