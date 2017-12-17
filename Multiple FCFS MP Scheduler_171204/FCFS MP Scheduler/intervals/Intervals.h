#pragma once

#include "..\globals\Globals.h"

#include <vector>
#include <algorithm>


using namespace std;

class Interval;

class Intervals
{
public:
	static const Intervals OPEN;


private:
	vector<Interval> intervals;

public:
	Intervals();

	Intervals(Interval baseline);	// for time baseline


	void	add(Interval newItv);	// for open intervals
	void	add2(Interval newItv);	// for closed intervals

	void	insert(int i, Interval newItv);
	void	set(int i, Interval newItv);
	void	erase(int i);

	int		getSize() const;	// return size of intervals
	void	clear();
	void	pushBack(Interval newItv);

	// accessor
	vector<Interval>	getAll() const;
	Interval			get(int i) const;
	Interval			getFirst() const;
	Interval			getLast() const;
	int					getMin() const;


	// operations
	Intervals	intersectionSet(Interval newItv);	// open operation for interval input	
	Intervals	intersectionSet2(Interval newItv);	// closed operation for interval input	
	Intervals	unionSet(Interval newItv);		// open operation for interval input
	Intervals	unionSet2(Interval newItv);		// closed operation for interval input

	Intervals	intersectionSets(Intervals newItvs);	// open operation for intervals input
	Intervals	intersectionSets2(Intervals newItvs);	// closed operation for intervals input

	Intervals	complementarySets();

	// for time transition (delta = transit time)
	Intervals	parallelOffset(int delta);
	Intervals	offset(int delta1, int delta2);


	// for link constraint operation
	Intervals	linkConstraints(Interval entry, Interval exit);		// forward
	Intervals	linkConstraints2(Interval exit, Interval entry);	// backward

	bool	exist(int n);

	void	print();
};

