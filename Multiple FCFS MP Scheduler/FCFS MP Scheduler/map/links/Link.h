#pragma once

#include "..\nodes\Node.h"
#include "..\..\counts\CountList.h"

#include <string>

using namespace std;

class Link
{
private:

	/* link informations */
	string	id;
	string	type;
	string	node1;
	string	node2;
	Length	length;
	
	/* link constraints */
	CountList	count;
	CountList	capacity;

	/* link time line */
	Intervals	positiveAvailableSlots;		// same direction for positive count
	Intervals	negativeAvailableSlots;		// opposite direction for negative count

public:

	Link();
	Link(string id);

	void	setType(string type);
	void	setNodes(string node1, string node2);
	void	setLength(Length length);
	
	string	getId() const;
	string	getType() const;
	string	getNode1() const;
	string	getNode2() const;
	Length	getLength() const;

	void	setCount(CountList count);
	void	setCapacity(CountList capacity);

	void	addCount(Count count);
	void	addCapacity(Count capacity);

	void	update(Interval interval, int direction);

	int		getCount(Time time) const;
	int		getCapacity(Time time) const;

	int		getCount(int i) const;		// 171114 추가(LHW) : for FileWrite
	int		getCapacity(int i) const;	// 171114 추가(LHW) : for FileWrite
	int		getCountTime(int i) const;	// 171114 추가(LHW) : for FileWrite
	int		getCountSize() const;		// 171114 추가(LHW) : for FileWrite
	
	void		setAvailableSlots();
	Intervals	getAvailableSlots(int direction);
};

