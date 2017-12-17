#include "Flight.h"



Flight::Flight() {}

Flight::Flight(int index, string id, string address, string type, string wakeCategory, string state, string connection) : index(index), id(id), address(address), type(type), wakeCategory(wakeCategory), state(state), connection(connection) {
	
	if (wakeCategory.empty()) {
		this->wakeCategory = "H";
	}
}

void Flight::setOriginalRoute(Route route) {
	originalRoute = route;
}

void Flight::setNewRoute(Route route) {
	newRoute = route;
}

int Flight::getIndex() const {
	return index;
}

string Flight::getId() const {
	return id;
}

string Flight::getAddress() const {
	return address;
}

string Flight::getType() const {
	return type;
}

string Flight::getWakeCategory() const {
	return wakeCategory;
}

string Flight::getState() const {
	return state;
}

string Flight::getConnection() const {
	return connection;
}

Route Flight::getOriginalRoute() const {
	return originalRoute;
}

Route Flight::getNewRoute() const {
	return newRoute;
}

Time Flight::getFirstEntryTime() const {
	return originalRoute.getFirstEntryTime();
}
 
void Flight::display() {

	cout << "*" << "Index" << " / " << "Callsign" << " / " << "Reg" << " / " << "Type" << " / " << "Class" << " / " << "State" << " / " << "Connection" << " / " << "From-To" << endl;
	cout << "  " << index << " / " << id << " / " << address << " / " << type << " / " << wakeCategory << " / " << state <<
		" / " << connection << " / " << originalRoute.getStartPoint() << "-" << originalRoute.getEndPoint() << "\n" << endl;
}

bool Flight::equals(Flight flight) {

	bool equality = false;

	/*if (id == flight.getId() && address == flight.getAddress() && state == flight.getState()) {
		equality = true;
	}*/

	if (index == flight.getIndex()) {
		equality = true;
	}

	return equality;
}

/* For multiple scheduling */
void Flight::setTempRoute(Route route) {
	tempRoute = route;
}

Route Flight::getTempRoute() const {
	return tempRoute;
}

bool Flight::getComplete() {
	return complete;
}

void Flight::setReset(bool reset) {
	this->reset = reset;
}

bool Flight::getReset() {
	return reset;
}

void Flight::setTempRoute(Time lookAhead, Links links) {
	tempRoute = originalRoute.cut(lookAhead, links);
}

void Flight::update(Time cycle, Links links) {
		
	bool cut = false;
	bool atNode = false;

	int cutPos = 0;

	Time entry, exit, transit;
	Time total = Time::ZERO;

	string cutPath, node, link;

	int speedUp = 0;
	int slowDown = 0;

	for (int i = 0; i < newRoute.getRouteSize(); i++) {

		entry = newRoute.getTimeSet(i).getEntryTime();
		exit = newRoute.getTimeSet(i).getExitTime();
		transit = newRoute.getTimeSet(i).getTransitTime();

		total = total.sum(transit);

		if (total.get(Time::PSEC) > cycle.get(Time::PSEC)) {	// update cycle is larger than total transit time

			transit = transit.subtract(total.subtract(cycle));
			exit = entry.sum(transit);
			
			cutPos = i;
			cut = true;
						
			speedUp = newRoute.getSpeedUp(cutPos);
			slowDown = newRoute.getSlowDown(cutPos);

			cutPath = newRoute.getPath(cutPos + 1);

			break;

		} else if (total.get(Time::PSEC) == cycle.get(Time::PSEC) && i < newRoute.getRouteSize() - 1) {

			cutPos = i;
			cut = true;
			atNode = true;

			cutPath = newRoute.getPath(cutPos + 1);

			break;
		}
	}

	if (cut == true) {
				
		if (cutPos < newRoute.getPathSize() - 3) {

			if (links.get(cutPath).getNode2() == links.get(newRoute.getPath(cutPos + 1)).getNode1()
				|| links.get(cutPath).getNode2() == links.get(newRoute.getPath(cutPos + 1)).getNode2()) {

				node = links.get(cutPath).getNode2();

			} else {
				node = links.get(cutPath).getNode1();
			}
		} else {
			node = newRoute.getEndPoint();
		}

		if (atNode == false) {
			node = node + "_TEMP";			
		}

		newRoute.eraseTimeSetFrom2End(cutPos);
		newRoute.eraseSpeedRateFrom2End(cutPos);
		newRoute.erasePathFrom2End(cutPos + 1);

		newRoute.addTimeSet(entry, exit, transit);
		newRoute.addSpeedRate(speedUp, slowDown);

		newRoute.addPath(cutPath);
		newRoute.addPath(node);
	
	} else {
		complete = true;
	}	
}

void Flight::mergeUpdated(const Flight& flight) {

	Route updated = flight.getNewRoute();

	if (newRoute.empty()) {	// not updated yet

		setNewRoute(updated);

	} else {

		int pathEnd = newRoute.getPathSize() - 1;
		int routeEnd = newRoute.getRouteSize() - 1;

		int _pathEnd = updated.getPathSize() - 1;
		int _routeEnd = updated.getRouteSize() - 1;

		bool cut = false;
		bool multi = false;

		string lastLink = newRoute.getPath(pathEnd - 1);
		string endNode = newRoute.getEndPoint();

		string _lastLink = updated.getPath(_pathEnd - 1);
		string _endNode = updated.getEndPoint();

		if (endNode.find("TEMP") != string::npos) {	// if the end node of updated route is a temporary node		
			cut = true;
		}

		if (lastLink == _lastLink && endNode == _endNode) {	// if the link is too long
			multi = true;
		}

		Time entry, exit, transit, lastTransit, transit_origin;

		if (cut == false) {

			// path
			newRoute.erasePathFrom2End(pathEnd);

			for (int i = 1; i < updated.getPathSize(); i++) {	// add next path
				newRoute.addPath(updated.getPath(i));
			}

			// time sets			
			for (int i = 0; i < updated.getRouteSize(); i++) {	// add next time sets
				newRoute.addTimeSet(updated.getTimeSet(i).getEntryTime(), updated.getTimeSet(i).getExitTime(), updated.getTimeSet(i).getTransitTime());
			}

			// speed rates
			for (int i = 0; i < updated.getRouteSize(); i++) {	// add the next speed rates
				newRoute.addSpeedRate(updated.getSpeedUp(i), updated.getSlowDown(i));
			}

		} else {	// start at the temporary node

			// path
			newRoute.erasePathFrom2End(pathEnd - 1);	// erase the last link and the node

			for (int i = 1; i < updated.getPathSize(); i++) {	// add next path
				newRoute.addPath(updated.getPath(i));
			}

			// time sets
			entry = newRoute.getEntryTime(routeEnd);			
			exit = updated.getExitTime(0);
			transit = exit.subtract(entry);

			if (multi == true) {	// store the last transit time for the multi-split link
				lastTransit = newRoute.getTransitTime(routeEnd);
			}

			newRoute.eraseTimeSetFrom2End(routeEnd);	// erase time set at the temporary node
			newRoute.addTimeSet(entry, exit, transit);	// merge time set 

			for (int i = 1; i < updated.getRouteSize(); i++) {	// add next time sets
				newRoute.addTimeSet(updated.getTimeSet(i).getEntryTime(), updated.getTimeSet(i).getExitTime(), updated.getTimeSet(i).getTransitTime());
			}

			// speed rates
			transit_origin = originalRoute.getTransitTime(routeEnd);

			if (multi == true) {

				int speedRate = updated.getScheduledSpeedRate(_routeEnd);

				double c1 = 1/(1 + (double)speedRate/100);

				double c2 = 1;

				if (speedRate < 0) {	// speed-up
					c2 = (double)floor(c1*100)/100;
				} else if (speedRate > 0) {	// slow-down
					c2 = (double)ceil(c1*100)/100;
				}
				int ratio = (int)(c2*100 - 100);

				transit_origin = lastTransit.sum(updated.getTransitTime(_routeEnd).proportion(ratio));
			}
			
			int nom = transit_origin.subtract(transit).get(Time::PSEC);
			int den = transit_origin.get(Time::PSEC);

			double speedRate = (double)nom / (double)den;

			int speedUp = 0;
			int slowDown = 0;

			if (speedRate > 0) {

				speedRate = (double)floor(speedRate*100)/100;

				speedUp = (int)(speedRate*100);
				slowDown = 0;

			} else if (speedRate < 0) {

				speedRate = (double)ceil(-speedRate*100)/100;

				speedUp = 0;
				slowDown = (int)(speedRate*100);

			} else {
				speedUp = 0;
				slowDown = 0;
			}

			newRoute.eraseSpeedRateFrom2End(routeEnd);
			newRoute.addSpeedRate(speedUp, slowDown);

			for (int i = 1; i < updated.getRouteSize(); i++) {	// add the next speed rates
				newRoute.addSpeedRate(updated.getSpeedUp(i), updated.getSlowDown(i));
			}
		}
	}

	/* if the schedule ends at the original end node */
	if (updated.getEndPoint() == originalRoute.getEndPoint()) {
		complete = true;
	}
}


void Flight::handleOverlap(const Flight& flight) {	// compare original route to updated new route

	Route scheduled = flight.getNewRoute();

	bool cut = false;

	string endNode = scheduled.getEndPoint();
	
	if (endNode.find("TEMP") != string::npos) {	// if the end node of updated route is a temporary node		
		cut = true;
	}

	string link = scheduled.getPath(scheduled.getPathSize() - 2);	// last link of updated new route

	int endIdx = scheduled.getRouteSize() - 1;

	int cutPos = 0;
	
	for (int i = 0; i < originalRoute.getPathSize(); i++) {
		if (originalRoute.getPath(i) == link) {
			cutPos = i;
			break;
		}
	}

	Time entry, exit, transit, offset;

	int speedUp = 0;
	int slowDown = 0;
	
	entry = scheduled.getLastExitTime();

	if (cut == false) {	// end node is a actual node
		
		transit = originalRoute.getTransitTime(cutPos);
		exit = entry.sum(transit);

		offset = exit.subtract(originalRoute.getExitTime(cutPos));
		originalRoute.timeOffset(offset);

		speedUp = originalRoute.getSpeedUp(cutPos);
		slowDown = originalRoute.getSlowDown(cutPos);

	} else {	// end node is a temporary node

		int speedRate = scheduled.getScheduledSpeedRate(endIdx);

		double c1 = 1/(1 + (double)speedRate/100);

		double c2 = 1;

		if (speedRate < 0) {	// speed-up
			c2 = (double)floor(c1*100)/100;
		} else if (speedRate > 0) {	// slow-down
			c2 = (double)ceil(c1*100)/100;
		} 
		int ratio = (int)(c2*100 - 100);

		Time t_scheduled = originalRoute.getTransitTime(cutPos - 1).proportion(speedRate);
		Time t_updated = scheduled.getTransitTime(endIdx);

		Time dt = t_scheduled.subtract(t_updated);

		transit = dt.proportion(ratio);

		exit = entry.sum(transit);

		if (cutPos == originalRoute.getPathSize() - 2) {	// if the split link equals to the last link
			offset = exit.subtract(originalRoute.getEntryTime(cutPos - 1));
		} else {
			offset = exit.subtract(originalRoute.getEntryTime(cutPos));
		}
				
		originalRoute.timeOffset(offset);	

		speedUp = originalRoute.getSpeedUp(cutPos - 1);
		slowDown = originalRoute.getSlowDown(cutPos - 1);
	}

	if (cut == false) {
		cutPos = cutPos + 1;
	} 

	originalRoute.erasePathFromTo(0, cutPos);
	originalRoute.eraseTimeFromTo(0, cutPos);
	originalRoute.eraseSpeedRateFromTo(0, cutPos);

	originalRoute.insertPath(endNode, 0);
	originalRoute.insertTimeSet(entry, exit, transit, 0);
	originalRoute.insertSpeedRate(speedUp, slowDown, 0);

	newRoute = scheduled;
}

void Flight::setOffset(Time timeOffset) {
	this->timeOffset = timeOffset;
}

Time Flight::getOffset() {
	return timeOffset;
}

void Flight::routeOffset() {
	originalRoute.timeOffset(timeOffset);
}