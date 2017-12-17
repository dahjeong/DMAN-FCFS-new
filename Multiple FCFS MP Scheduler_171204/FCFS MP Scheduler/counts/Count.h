#pragma once

#include "..\intervals\Interval.h"
#include "..\physicalProperty\Time.h"

class Count
{

private:
	Time	time;
	int		count;

public:
	Count(Time time, int count);

	Time	getTime() const;
	int		getCount() const;

	void	setCount(int count);

	void	print();
};

