#pragma once

#include "Intervals.h"
#include "..\globals\Globals.h"

#include <iostream>



class Interval
{
private:
	int begin;
	int end;


public:
	static const Interval NULLSET;	// null set of set operation (АјС§Че)

	Interval();
	Interval(int begin, int end);
	
	int	getBegin() const;
	int	getEnd() const;

	bool	equals(Interval newItv);

	// opearations
	Interval	intersectionSet(Interval newItv);	// for open interval
	Interval	intersectionSet2(Interval newItv);	// for closed interval
	
	Intervals	unionSet(Interval newItv);
	Intervals	unionSet2(Interval newItv);

	Intervals	complementarySet();

	Interval	parallelOffset(int delta);
	Interval	offset(int delta1, int delta2);

	bool		exist(int n);

	void		print();
};

