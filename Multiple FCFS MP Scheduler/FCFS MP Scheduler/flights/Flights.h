#pragma once

#include "Flight.h"

#include <vector>
#include <algorithm>

#include <iostream>

using namespace std;


class Flights
{
private:
	vector<Flight> flights;

public:
	Flights();
	~Flights();
	
	void	add(Flight flight);

	void	clear();

	void	erase(int i);

	int		getSize() const;

	Flight	get(int i) const;
	Flight	get(string id, string address, string state) const;

	Time	getFirstEntryTime();

	/* prioritization */
	void	sort_Priority_Nominal();
	void	sort_Priority_Arrival();
	void	sort_Priority_Partial_Arrival(Time window);


	/* For multiple scheduling */
	int		getCompletedSize() const;

	Flights	rangeOf(Time window);
	
	void	reconfigure(Time lookAhead, Links links);

	void	update(Time cycle, Links links);	// the function to update flights by its schedule
	
	void	mergeUpdated(Flights newFlights);	// the function to merge scheduled and updated flights

	void	handleOverlap(Flights newFlights);	// the function to handle remained flights and scheduled flights,
												// by deleting overlapped original schedule.

	void	handleComplete(Flights newFlights);

	void	eraseResets();
};

