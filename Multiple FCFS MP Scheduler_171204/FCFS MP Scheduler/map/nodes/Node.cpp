#include "Node.h"

/* Inner class TimeStrip */
Node::TimeStrip::TimeStrip() : time(Time::NaN) {}

Node::TimeStrip::TimeStrip(Time time, int index, string state, string wakeCategory, int direction) : time(time), index(index), state(state), wakeCategory(wakeCategory), direction(direction) {}

Time Node::TimeStrip::getTime() const {
	return time;
}

int Node::TimeStrip::getIndex() const {
	return index;
}

string Node::TimeStrip::getState() const {
	return state;
}

string Node::TimeStrip::getWakeCatetory() const {
	return wakeCategory;
}

int Node::TimeStrip::getDirection() const {
	return direction;
}


/* class Node */
Node::Node() {}

Node::Node(string id) : id(id) {
	availableSlots.add(Interval(Time::TIMEMIN.get(Time::PSEC), Time::TIMEMAX.get(Time::PSEC)));
}

void Node::setType(string type) {
	this->type = type;
}

void Node::setLatLon(LatLon latlon) {
	this->latlon = latlon;
}

void Node::setComment(string comment) {
	this->comment = comment;
}

string Node::getId() const {
	return id;
}

string Node::getType() const {
	return type;
}

LatLon Node::getLatLon() const {
	return latlon;
}

string Node::getComment() const {
	return comment;
}

void Node::setDepRate(CountList depRate) {
	this->depRate = depRate;
}

void Node::setArrRate(CountList arrRate) {
	this->arrRate = arrRate;
}

void Node::setPassRate(CountList passRate) {
	this->passRate = passRate;
}

CountList Node::getDepRate() {
	return depRate;
}

CountList Node::getArrRate() {
	return arrRate;
}

CountList Node::getPassRate() {
	return passRate;
}

void Node::addDepRate(Count depRate) {
	this->depRate.add(depRate);
}

void Node::addArrRate(Count arrRate) {
	this->arrRate.add(arrRate);
}

void Node::addPassRate(Count passRate) {
	this->passRate.add(passRate);
}

int Node::getDepRateN(Time time) {
	return depRate.getCountN(time);
}

int Node::getArrRateN(Time time) {
	return arrRate.getCountN(time);
}

int Node::getPassRateN(Time time) {
	return passRate.getCountN(time);
}


void Node::setAvailableSlots(Interval closed) {
	availableSlots = availableSlots.intersectionSets2(closed.complementarySet());
}

Intervals Node::getAvailableSlots() {
	return availableSlots;
}

Intervals Node::getAvailableSlots2(int index, string state, string wakeCategory, SeparationMinima sepMinima) {
	
	Intervals availables, unavailables;

	int direction = 1;

	Time time, dt1, dt2; 
	int begin = 0;
	int end = 0;

	/* To calculate available slots of runway threshold node */
	if (timeStrips.empty()) {

		availables = Intervals::OPEN;

	} else if (type.find("RWY") != string::npos) {
		
		for (int i = 0; i < timeStrips.size(); i++) {

			if (index != timeStrips.at(i).getIndex()) {

				time = timeStrips.at(i).getTime();

				if (comment.size() > 1 || timeStrips.at(i).getState() == "DEP") {

					dt1 = sepMinima.getMinSepTime(state, timeStrips.at(i).getState(),
						wakeCategory, timeStrips.at(i).getWakeCatetory(),
						direction, timeStrips.at(i).getDirection());

					dt2 = sepMinima.getMinSepTime(timeStrips.at(i).getState(), state,
						timeStrips.at(i).getWakeCatetory(), wakeCategory,
						timeStrips.at(i).getDirection(), direction);

					begin = time.subtract(dt1).get(Time::PSEC);
					end = time.sum(dt2).get(Time::PSEC);

					if (begin < 0) {
						begin = 0;
					}
					if (end < 0) {
						end = 0;
					}
					unavailables.add(Interval(begin, end));				
				} 			
			}
		}

		if (unavailables.getSize() > 0) {
			availables = unavailables.complementarySets();
		} else {
			availables = Intervals::OPEN;			
		}

	} 
	return availables;
}

void Node::update(Time time, int index, string state, string wakeCategory, int direction) {
	
	TimeStrip timeStrip(time, index, state, wakeCategory, direction);
	
	// time sorting

	if (timeStrips.empty()) {

		timeStrips.push_back(timeStrip);
	
	} else {

		int oldSize = getStripSize();

		for (int i = 0; i < oldSize; i++) {

			Time oldTime = timeStrips.at(i).getTime();

			if (oldTime.get(Time::PSEC) > time.get(Time::PSEC)) {

				timeStrips.insert(timeStrips.begin() + i, timeStrip);
				break;

			} else {
				if (i == oldSize - 1) {
					timeStrips.push_back(timeStrip);
				}
			}
		}
	}


	if (comment.size() <= 1) {	// all nodes except threshold

		int rate = 0;
		int block = 0;

		if (type.find("Taxi") != string::npos) {	// taxi node	

			rate = getPassRateN(time);	
			block = (int) round((double)Time(1, Time::HOUR).get(Time::PSEC)/(double)rate + 0.5);
			setAvailableSlots(Interval(time.get(Time::PSEC) - block, time.get(Time::PSEC) + block));

		} else if (type.find("Gate") != string::npos) {	// gate node

			if (state == "DEP") {
				rate = getDepRateN(time);
			} else {
				rate = getArrRateN(time);
			}

			block = (int) round((double)Time(1, Time::HOUR).get(Time::PSEC)/(double)rate + 0.5);
			setAvailableSlots(Interval(time.get(Time::PSEC) - block, time.get(Time::PSEC) + block));

		} else if (type.find("RWY") != string::npos && state == "ARR") {

			rate = getPassRateN(time);
			block = (int) round((double)Time(1, Time::HOUR).get(Time::PSEC)/(double)rate + 0.5);
			setAvailableSlots(Interval(time.get(Time::PSEC) - block, time.get(Time::PSEC) + block));
		}

	}
}

Node::TimeStrip Node::getStrip(int i) const {
	return timeStrips.at(i);
}

Node::TimeStrip Node::getStripByIdx(int index) const {

	TimeStrip timeStrip;

	for (int i = 0; i < timeStrips.size(); i++) {

		if (timeStrips.at(i).getIndex() == index) {
			timeStrip = timeStrips.at(i);
			break;
		}
	}
	return timeStrip;
}

Node::TimeStrip Node::getLastStrip() const {
	return timeStrips.back();
}

Time Node::getTime(int i) const {
	return timeStrips.at(i).getTime();
}

string Node::getState(int i) const {
	return timeStrips.at(i).getState();
}

string Node::getWakeCategory(int i) const {
	return timeStrips.at(i).getWakeCatetory();
}

int Node::getDirection(int i) const {
	return timeStrips.at(i).getDirection();
}


Time Node::getTimeByIdx(int index) const {
	return getStripByIdx(index).getTime();
}

string Node::getStateByIdx(int index) const {
	return getStripByIdx(index).getState();
}

string Node::getWakeCategoryByIdx(int index) const {
	return getStripByIdx(index).getWakeCatetory();
}

int Node::getDirectionByIdx(int index) const {
	return getStripByIdx(index).getDirection();
}



int Node::getStripSize() const {
	return static_cast<int>(timeStrips.size());
}

/* for multiple scheduling */
bool Node::selfBlock(int index) {

	bool self = false;

	for (int i = 0; i < timeStrips.size(); i++) {

		if (index == timeStrips.at(i).getIndex()) {
			self = true;
			break;
		}
	}

	return self;
}

void Node::erase(Time time, int index, string state, string wakeCategory, int direction) {

	if (timeStrips.empty()) {

	} else {

		int oldSize = getStripSize();

		for (int i = 0; i < oldSize; i++) {

			if (index == timeStrips.at(i).getIndex()) {
				timeStrips.erase(timeStrips.begin() + i);
				break;
			}
		}
	}

	if (comment.size() <= 1) {	// all nodes except threshold

		int rate = 0;
		int block = 0;

		if (type.find("Taxi") != string::npos) {	// taxi node	

			rate = getPassRateN(time);
			block = (int)round((double)Time(1, Time::HOUR).get(Time::PSEC)/(double)rate + 0.5);
			availableSlots = availableSlots.unionSet2(Interval(time.get(Time::PSEC) - block, time.get(Time::PSEC) + block));

		} else if (type.find("Gate") != string::npos) {	// gate node

			if (state == "DEP") {
				rate = getDepRateN(time);
			} else {
				rate = getArrRateN(time);
			}

			block = (int)round((double)Time(1, Time::HOUR).get(Time::PSEC)/(double)rate + 0.5);
			availableSlots = availableSlots.unionSet2(Interval(time.get(Time::PSEC) - block, time.get(Time::PSEC) + block));

		} else if (type.find("RWY") != string::npos && state == "ARR") {

			rate = getPassRateN(time);
			block = (int)round((double)Time(1, Time::HOUR).get(Time::PSEC)/(double)rate + 0.5);
			availableSlots = availableSlots.unionSet2(Interval(time.get(Time::PSEC) - block, time.get(Time::PSEC) + block));
		}
	}
}