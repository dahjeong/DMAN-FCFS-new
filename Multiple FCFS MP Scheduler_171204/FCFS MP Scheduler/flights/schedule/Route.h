#pragma once

#include "..\..\physicalProperty\Time.h"
#include "..\..\map\links\Links.h"

#include <vector>
#include <algorithm>
#include <string>

using namespace std;

class Route
{
public:
	class TimeSet {

	private:
		Time entryTime;
		Time exitTime;
		Time transitTime;

	public:
		TimeSet();
		TimeSet(Time entryTime, Time exitTime, Time transitTime);

		Time getEntryTime() const;
		Time getExitTime() const;
		Time getTransitTime() const;

		/* For multiple scheduling */
		void offset(Time offset);
	};

public:
	class SpeedRate {

	private:
		int speedUp;
		int slowDown;

	public:
		SpeedRate();
		SpeedRate(int speedUp, int slowDown);

		int getSpeedUp() const;
		int getSlowDown() const;
	};



/* scheduled route for a single flight */
private:	

	string	id;
	vector<TimeSet>		times;
	vector<string>		path;
	vector<SpeedRate>	speedRates;


public:
	Route();
	Route(string id);

	/* initialize */
	void	setId(string id);
	void	addTimeSet(const Time& entryTime, const Time& exitTime, const Time& transitTime);
	void	addPath(string path);
	void	addSpeedRate(int speedUp, int slowDown);

	void	clear();

	/* accessors */
	string	getId() const;

	int		getRouteSize() const;
	int		getPathSize() const;

	string	getPath(int i) const;
	string	getStartPoint() const;
	string	getEndPoint() const;

	TimeSet	getTimeSet(int i) const;

	Time	getEntryTime(int i) const;
	Time	getExitTime(int i) const;
	Time	getTransitTime(int i) const;

	Time	getFirstEntryTime() const;
	Time	getLastExitTime() const;

	Time	getTotalTransitTime() const;

	SpeedRate	getSpeedRate(int i) const;

	int		getSpeedUp(int i) const;
	int		getSlowDown(int i) const;
	int		getScheduledSpeedRate(int i) const;

	bool	empty() const;

	/* For multiple scheduling */
	Route	cut(Time window, Links links);	// cut route by time window(look ahead time)

	void	erasePathAt(int i);
	void	eraseTimeSetAt(int i);
	void	eraseSpeedRateAt(int i);

	void	erasePathFrom2End(int i);
	void	eraseTimeSetFrom2End(int i);
	void	eraseSpeedRateFrom2End(int i);

	void	erasePathFromTo(int begin, int end);
	void	eraseTimeFromTo(int begin, int end);
	void	eraseSpeedRateFromTo(int begin, int end);

	void	insertPath(string path, int i);
	void	insertTimeSet(const Time& entryTime, const Time& exitTime, const Time& transitTime, int i);
	void	insertSpeedRate(int speedUp, int slowDown, int i);

	void	timeOffset(Time offset);
};

