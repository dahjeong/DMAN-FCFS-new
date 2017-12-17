#include "Route.h"

/* Inner class TimeSet */
Route::TimeSet::TimeSet() : entryTime(Time::NaN), exitTime(Time::NaN), transitTime(Time::NaN) {}

Route::TimeSet::TimeSet(Time entryTime, Time exitTime, Time transitTime) : entryTime(entryTime), exitTime(exitTime), transitTime(transitTime) {}

Time Route::TimeSet::getEntryTime() const {
	return entryTime;
}

Time Route::TimeSet::getExitTime() const {
	return exitTime;
}

Time Route::TimeSet::getTransitTime() const {
	return transitTime;
}

void Route::TimeSet::offset(Time offset) {
	entryTime = entryTime.sum(offset);
	exitTime = exitTime.sum(offset);
	transitTime = exitTime.subtract(entryTime);
}

/* Inner class SpeedRate */
Route::SpeedRate::SpeedRate() : speedUp(0), slowDown(0) {}

Route::SpeedRate::SpeedRate(int speedUp, int slowDown) : speedUp(speedUp), slowDown(slowDown) {}

int Route::SpeedRate::getSpeedUp() const {
	return speedUp;
}

int Route::SpeedRate::getSlowDown() const {
	return slowDown;
}

/* class Route */
Route::Route() {}

Route::Route(string id) : id(id) {}

void Route::setId(string id) {
	this->id = id;
}

void Route::addTimeSet(const Time& entryTime, const Time& exitTime, const Time& transitTime) {
	times.push_back(TimeSet{ entryTime, exitTime, transitTime });
}

void Route::addPath(string path) {

	if (path != "XXXX") {

		if (this->path.empty()) {
			
			this->path.push_back(path);
		
		} else {

			bool exist = false;

			for (int i = 0; i < this->path.size(); i++) {

				if (this->path.at(i) == path && this->path.at(i).find("TEMP") == string::npos) {
					exist = true;
					break;
				}
			}

			if (exist == false) {
				this->path.push_back(path);
			}
		}
	}
}

void Route::addSpeedRate(int speedUp, int slowDown) {
	speedRates.push_back(SpeedRate{ speedUp, slowDown });
}

void Route::clear() {
	id.clear();
	times.clear();
	path.clear();
	speedRates.clear();
}

string Route::getId() const {
	return id;
}

int Route::getRouteSize() const {
	return static_cast<int>(times.size());
}

int Route::getPathSize() const {
	return static_cast<int>(path.size());
}

string Route::getPath(int i) const {
	return path.at(i);
}

string Route::getStartPoint() const {
	return path.front();
}

string Route::getEndPoint() const {
	return path.back();
}

Route::TimeSet Route::getTimeSet(int i) const {
	return times.at(i);
}

Time Route::getEntryTime(int i) const {
	return times.at(i).getEntryTime();
}

Time Route::getExitTime(int i) const {
	return times.at(i).getExitTime();
}

Time Route::getTransitTime(int i) const {
	return times.at(i).getTransitTime();
}

Time Route::getFirstEntryTime() const {
	return times.front().getEntryTime();
}

Time Route::getLastExitTime() const {
	return times.back().getExitTime();
}

Time Route::getTotalTransitTime() const {

	Time total = Time::ZERO;

	for (int i = 0; i < (times.size()); i++) {

		total = total.sum(times.at(i).getTransitTime());
	}

	return total;
}

Route::SpeedRate Route::getSpeedRate(int i) const {
	return speedRates.at(i);
}

int Route::getSpeedUp(int i) const {
	return speedRates.at(i).getSpeedUp();
}

int Route::getSlowDown(int i) const {
	return speedRates.at(i).getSlowDown();
}

int Route::getScheduledSpeedRate(int i) const {

	int speedRate = 0;

	if (getSpeedUp(i) == 0 && getSlowDown(i) != 0) {
		speedRate = getSlowDown(i);
	} else if (getSpeedUp(i) != 0 && getSlowDown(i) == 0) {
		speedRate = -getSpeedUp(i);
	}
		
	return speedRate;
}

bool Route::empty() const {
	
	bool empty = false;

	if (times.empty() && path.empty() && speedRates.empty()) {
		empty = true;
	}

	return empty;
}

/* For multiple scheduling */
Route Route::cut(Time window, Links links) {

	Route part(id);

	vector<TimeSet>	_times = times;
	vector<SpeedRate> _speedRates = speedRates;
	vector<string> _path = path;

	bool cut = false;
	bool atNode = false;

	int splitPos = 0;

	Time entry, exit, transit;
	Time total = Time::ZERO;

	string node, link;

	/* split times, speeds */
	for (int i = 0; i < getRouteSize(); i++) {
				
		entry = _times.at(i).getEntryTime();
		exit = _times.at(i).getExitTime();
		transit = _times.at(i).getTransitTime();

		total = total.sum(_times.at(i).getTransitTime());
		
		if (total.get(Time::PSEC) > window.get(Time::PSEC)) {

			transit = transit.subtract(total.subtract(window));
			exit = entry.sum(transit);

			splitPos = i;
			cut = true;

			break;

		} else if (total.get(Time::PSEC) == window.get(Time::PSEC) && i < getRouteSize() - 1) {

			splitPos = i + 1;
			cut = true;
			atNode = true;

			break;
		}
	}

	if (cut == true) {
				
		_times.erase(_times.begin() + splitPos, _times.end());
		_times.push_back(TimeSet(entry, exit, transit));

		_speedRates.erase(_speedRates.begin() + _times.size(), _speedRates.end());

		/* split path */
		splitPos = splitPos + 1;

		link = _path.at(splitPos);				
		
		if (splitPos < _path.size() - 2) {

			if (links.get(_path.at(splitPos)).getNode2() == links.get(_path.at(splitPos + 1)).getNode1()
				|| links.get(_path.at(splitPos)).getNode2() == links.get(_path.at(splitPos + 1)).getNode2()) {

				node = links.get(_path.at(splitPos)).getNode2();

			} else {
				node = links.get(_path.at(splitPos)).getNode1();
			}
		} else {
			node = _path.back();
		}

		if (atNode == false) {
			node = node + "_TEMP";
		}
		
		_path.erase(_path.begin() + splitPos + 1, _path.end());
		_path.push_back(node);
	}

	for (int k = 0; k < _path.size(); k++) {

		part.addPath(_path.at(k));

		if (k < _times.size()) {
			part.addTimeSet(_times.at(k).getEntryTime(), _times.at(k).getExitTime(), _times.at(k).getTransitTime());
			part.addSpeedRate(_speedRates.at(k).getSpeedUp(), _speedRates.at(k).getSlowDown());
		}		
	}

	return part;
}

void Route::erasePathAt(int i) {
	path.erase(path.begin() + i);
}

void Route::eraseTimeSetAt(int i) {
	times.erase(times.begin() + i);
}

void Route::eraseSpeedRateAt(int i) {
	speedRates.erase(speedRates.begin() + i);
}

void Route::erasePathFrom2End(int i) {
	path.erase(path.begin() + i, path.end());
}

void Route::eraseTimeSetFrom2End(int i) {
	times.erase(times.begin() + i, times.end());
}

void Route::eraseSpeedRateFrom2End(int i) {
	speedRates.erase(speedRates.begin() + i, speedRates.end());
}

void Route::erasePathFromTo(int begin, int end) {
	path.erase(path.begin() + begin, path.begin() + end);
}

void Route::eraseTimeFromTo(int begin, int end) {
	times.erase(times.begin() + begin, times.begin() + end);
}

void Route::eraseSpeedRateFromTo(int begin, int end) {
	speedRates.erase(speedRates.begin() + begin, speedRates.begin() + end);
}

void Route::insertPath(string path, int i) {

	if (path != "XXXX") {

		if (this->path.empty()) {

			this->path.push_back(path);

		} else {

			bool exist = false;

			for (int j = 0; j < this->path.size(); j++) {

				if (this->path.at(j) == path) {
					exist = true;
					break;
				}
			}

			if (exist == false) {
				this->path.insert(this->path.begin() + i, path);
			}
		}
	}
}

void Route::insertTimeSet(const Time& entryTime, const Time& exitTime, const Time& transitTime, int i) {

	if (times.empty()) {
		times.push_back(TimeSet{ entryTime, exitTime, transitTime });
	} else {
		times.insert(times.begin() + i, TimeSet{ entryTime, exitTime, transitTime });
	}
}

void Route::insertSpeedRate(int speedUp, int slowDown, int i) {

	if (speedRates.empty()) {
		speedRates.push_back(SpeedRate{ speedUp, slowDown });
	} else {
		speedRates.insert(speedRates.begin() + i, SpeedRate{ speedUp, slowDown });
	}
}

void Route::timeOffset(Time offset) {

	for (int i = 0; i < getRouteSize(); i++) {
		times.at(i).offset(offset);
	}
}