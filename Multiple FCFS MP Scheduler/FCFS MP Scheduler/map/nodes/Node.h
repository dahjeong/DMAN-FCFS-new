#pragma once

#include "..\..\coordinate\LatLon.h"
#include "..\..\counts\CountList.h"
#include "..\..\scheduler\separationMinima\SeparationMinima.h"

#include <string>
#include <iostream>

using namespace std;

class Node
{
public:
	class TimeStrip {

	private:
		Time	time;
		int		index;	// aircraft index
		string	state;
		string	wakeCategory;
		int		direction;		// 1: same, -1: opposite (runway node), 0: no-direction (other)

	public:
		TimeStrip();
		TimeStrip(Time time, int index, string state, string wakeCategory, int direction);

		Time	getTime() const;
		int		getIndex() const;
		string	getState() const;
		string	getWakeCatetory() const;
		int		getDirection() const;
	};

private:
	
	/* node informations */
	string id;		// node id
	string type;	// type: Gate or Ramp or Taxi or Runway
	LatLon latlon;	// coordinate
	string comment;	// threshold or not
	
	/* node rate constraints */ 
	CountList depRate;	// for gate and runway nodes
	CountList arrRate;	// for gate and runway nodes
	CountList passRate;	// for junction nodes

	/* node time line */
	Intervals availableSlots;

	/* node time strips (flags) */
	vector<TimeStrip> timeStrips;


public:
	Node();
	Node(string id);

	void	setType(string type);
	void	setLatLon(LatLon latlon);
	void	setComment(string comment);
	
	string	getId() const;
	string	getType() const;
	LatLon	getLatLon() const;
	string	getComment() const;

	void	setDepRate(CountList depRate);
	void	setArrRate(CountList arrRate);
	void	setPassRate(CountList passRate);

	CountList	getDepRate();
	CountList	getArrRate();
	CountList	getPassRate();

	void	addDepRate(Count depRate);
	void	addArrRate(Count arrRate);
	void	addPassRate(Count passRate);

	int		getDepRateN(Time time);
	int		getArrRateN(Time time);
	int		getPassRateN(Time time);

	void		setAvailableSlots(Interval closed);

	Intervals	getAvailableSlots();
	Intervals	getAvailableSlots2(int index, string state, string wakeCategory, SeparationMinima sepMinima);

	void		update(Time time, int index, string state, string wakeCategory, int direction);

	TimeStrip	getStrip(int i) const;
	TimeStrip	getStripByIdx(int index) const;

	TimeStrip	getLastStrip() const;

	Time		getTime(int i) const;	
	string		getState(int i) const;
	string		getWakeCategory(int i) const;
	int			getDirection(int i) const;
	int			getStripSize() const;

	Time		getTimeByIdx(int index) const;
	string		getStateByIdx(int index) const;
	string		getWakeCategoryByIdx(int index) const;
	int			getDirectionByIdx(int index) const;


	/* for multiple scheduling */
	bool		selfBlock(int index);
	void		erase(Time time, int index, string state, string wakeCategory, int direction);
};

