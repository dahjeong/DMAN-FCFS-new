#pragma once

#include "Link.h"

#include <vector>
#include <algorithm>

using namespace std;

class Links
{
private:
	vector<Link> links;

public:
	Links();
	
	void	add(Link link);
	void	erase(int i);

	void	clear();

	int		getSize() const;

	Link	get(int i) const;
	Link	get(string id) const;

	void	setInitialConstraints(string id, Count count, Count capacity);

	void	setInitialAvailableSlots();

	void	update(string id, Interval interval, int direction);

	/* for multiple scheduling */
	void	erase(string id, Interval interval, int direction);
};

