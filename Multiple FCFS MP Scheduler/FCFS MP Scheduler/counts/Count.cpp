#include "Count.h"



Count::Count(Time time, int count) : time(time), count(count)
{
}

Time Count::getTime() const {
	return time;
}

int Count::getCount() const {
	return count;
}

void Count::setCount(int count) {
	this->count = count;
}

void Count::print() {
	printf("(%d sec, %d)\n", time.get(Time::SEC), count);
}