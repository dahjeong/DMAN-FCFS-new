#pragma once

#include "Count.h"

#include <vector>
#include <algorithm>


using namespace std;

class CountList
{
private:
	vector<Count> countList;

public:
	CountList();
	
	void	add(Count count);

	int		getSize() const;

	void	clear();

	Count	get(int i) const;
	Count	getFirst() const;
	Count	getLast() const;

	// get count of Count
	int		getCountN(int i) const;
	int		getCountN(Time time) const;
	int		getFirstCountN() const;
	int		getLastCountN() const;

	// operation between two countlists
	CountList	subtract(CountList target);
	CountList	add(CountList target);
	CountList	reverse();


	// convert to intervals
	Intervals	toIntervals(int maxCount, int direction);
	Intervals	toIntervals(int maxCount);

	// update
	void	update(Interval interval, int direction);

	void	print();

};

