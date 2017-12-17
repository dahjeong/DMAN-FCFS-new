#pragma once

#include "schedule\Route.h"


#include <string>
#include <iostream>

using namespace std;


class Flight
{
private:
	int		index;			// index
	string	id;				// callsign
	string	address;		// registration
	string	type;			// aircraft type
	string	wakeCategory;	// ICAO aircraft wake category
	string	state;			// departure or arrival
	string	connection;		// connecting flight (Y/N)
	
	Route	originalRoute;	// originally scheduled route
	Route	newRoute;		// new scheduled route after scheduling process

	/* For multiple scheduling */
	Route	tempRoute;		// temporary route derived from originally scheduled route

	bool	complete = false;	// flag for scheduling complete
	bool	reset = false;		// flag for rescheduling
	Time	timeOffset;				// offset time for reset

public:
	Flight();
	Flight(int index, string id, string address, string type, string wakeCategory, string state, string connection);

	/* initialize */
	void	setOriginalRoute(Route route);	
	void	setNewRoute(Route route);
	
	/* accessors */
	int		getIndex() const;
	string	getId() const;
	string	getAddress() const;
	string	getType() const;	
	string	getWakeCategory() const;
	string	getState() const;
	string	getConnection() const;

	Route	getOriginalRoute() const;
	Route	getNewRoute() const;

	Time	getFirstEntryTime() const;
	
	void	display();
	bool	equals(Flight flight);

	/* For multiple scheduling */
	void	setTempRoute(Route route);
	Route	getTempRoute() const;

	bool	getComplete();

	void	setReset(bool reset);
	bool	getReset();
	
	void	setTempRoute(Time lookAhead, Links links);
	void	update(Time cycle, Links links);

	void	mergeUpdated(const Flight& flight);

	void	handleOverlap(const Flight& flight);

	void	setOffset(Time timeOffset);
	Time	getOffset();

	void	routeOffset();
};

