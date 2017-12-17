#include "Link.h"



Link::Link() {}

Link::Link(string id) : id(id) {}

void Link::setType(string type) {
	this->type = type;
}

void Link::setNodes(string node1, string node2) {
	this->node1 = node1;
	this->node2 = node2;
}

void Link::setLength(Length length) {
	this->length = length;
}

string Link::getId() const {
	return id;
}

string Link::getType() const {
	return type;
}

string Link::getNode1() const {
	return node1;
}

string Link::getNode2() const {
	return node2;
}

Length Link::getLength() const {

	return length;
}

void Link::setCount(CountList count) {
	this->count = count;
}

void Link::setCapacity(CountList capacity) {
	this->capacity = capacity;
}

void Link::addCount(Count count) {
	this->count.add(count);
}

void Link::addCapacity(Count capacity) {
	this->capacity.add(capacity);
}

void Link::update(Interval interval, int direction) {
	count.update(interval, direction);
	setAvailableSlots();
}

int Link::getCount(Time time) const {
	return abs(this->count.getCountN(time));
}

int Link::getCapacity(Time time) const {
	return abs(this->capacity.getCountN(time));
}

int Link::getCount(int i) const {  //171114 추가(LHW) : for FileWrite
	return count.getCountN(i);
}

int Link::getCapacity(int i) const { //171114 추가(LHW) : for FileWrite
	return abs(capacity.getCountN(i));
}

int Link::getCountTime(int i) const { //171114 추가(LHW) : for FileWrite
	return count.get(i).getTime().get(Time::SEC);
}

int Link::getCountSize() const { //171114 추가(LHW) : for FileWrite
	return count.getSize();
}

void Link::setAvailableSlots() {
		
	CountList subtracted1 = capacity.subtract(count);
	CountList subtracted2 = capacity.add(count);

	int max = capacity.getFirstCountN();
	
	positiveAvailableSlots = subtracted1.toIntervals(max);
	negativeAvailableSlots = subtracted2.toIntervals(max);
}

Intervals Link::getAvailableSlots(int direction) {

	if (direction == 1) {
		return positiveAvailableSlots;
	} else {
		return negativeAvailableSlots;
	}
}