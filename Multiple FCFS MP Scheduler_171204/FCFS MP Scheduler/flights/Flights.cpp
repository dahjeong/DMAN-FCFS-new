#include "Flights.h"



Flights::Flights() {}

Flights::~Flights() {
	flights.~vector<Flight>();
}

void Flights::add(Flight flight) {

	if (flights.empty()) {
		
		flights.push_back(flight);
	
	} else {
		
		bool exist = false;

		for (int i = 0; i < getSize(); i++) {

			if (flights.at(i).equals(flight)) {
				exist = true;
				break;
			}
		}

		if (exist == false) {
			flights.push_back(flight);
		}
	}
}

void Flights::clear() {
	flights.clear();
}

void Flights::erase(int i) {
	flights.erase(flights.begin() + i);
}

int Flights::getSize() const {
	return static_cast<int>(flights.size());
}

Flight Flights::get(int i) const {
	return flights.at(i);
}

Flight Flights::get(string id, string address, string state) const {

	Flight flight;

	for (int i = 0; i < getSize(); i++) {

		if (flights.at(i).getId() == id && flights.at(i).getAddress() == address && flights.at(i).getState() == state) {

			flight = flights.at(i);
		
		} else {

			if (i == getSize() - 1) {

				cout << "Flight " << id << "(" << address << ")" << " does not exist.\n" << endl;
				system("pause");
			}
		}
	}
	return flight;
}

Time Flights::getFirstEntryTime() {
	return flights.front().getFirstEntryTime();
}


void Flights::sort_Priority_Nominal() {	// order of originally scheduled departure or arrival time 

	vector<Flight> sorted;

	for (int i = 0; i < getSize(); i++) {

		Time firstEntryTime = flights.at(i).getFirstEntryTime();

		if (sorted.empty()) {

			sorted.push_back(flights.at(i));

		} else {

			for (int j = 0; j < (sorted.size()); j++) {

				if (firstEntryTime.get(Time::PSEC) <= sorted.at(j).getFirstEntryTime().get(Time::PSEC)) {

					sorted.insert(sorted.begin() + j, flights.at(i));
					break;
				
				} else {

					if (j == (sorted.size()) - 1) {

						sorted.push_back(flights.at(i));
						break;
					}
				}
			}
		}
	}
	flights.swap(sorted);
}

void Flights::sort_Priority_Arrival() {	// arrival flight first 

	vector<Flight> sorted_DEP;
	vector<Flight> sorted_ARR;

	for (int i = 0; i < getSize(); i++) {

		if (flights.at(i).getState() == "DEP") {

			Time firstEntryTime = flights.at(i).getFirstEntryTime();

			if (sorted_DEP.empty()) {

				sorted_DEP.push_back(flights.at(i));

			} else {

				for (int d = 0; d < (sorted_DEP.size()); d++) {

					if (firstEntryTime.get(Time::PSEC) <= sorted_DEP.at(d).getFirstEntryTime().get(Time::PSEC)) {

						sorted_DEP.insert(sorted_DEP.begin() + d, flights.at(i));
						break;

					} else {

						if (d == (sorted_DEP.size()) - 1) {

							sorted_DEP.push_back(flights.at(i));
							break;
						}
					}
				}
			}

		} else {

			Time firstEntryTime = flights.at(i).getFirstEntryTime();

			if (sorted_ARR.empty()) {

				sorted_ARR.push_back(flights.at(i));

			} else {

				for (int a = 0; a < (sorted_ARR.size()); a++) {

					if (firstEntryTime.get(Time::PSEC) <= sorted_ARR.at(a).getFirstEntryTime().get(Time::PSEC)) {

						sorted_ARR.insert(sorted_ARR.begin() + a, flights.at(i));
						break;

					} else {

						if (a == (sorted_ARR.size()) - 1) {

							sorted_ARR.push_back(flights.at(i));
							break;
						}
					}
				}
			}
		}
	}

	sorted_ARR.insert(sorted_ARR.end(), sorted_DEP.begin(), sorted_DEP.end());

	flights.swap(sorted_ARR);
}

void Flights::sort_Priority_Partial_Arrival(Time window) {

	vector<Flight> sorted;

	Flights part;

	sort_Priority_Nominal();

	while (1) {

		if (getSize() == 0) {
			break;
		}

		part = rangeOf(window);
		part.sort_Priority_Arrival();

		for (int i = 0; i < part.getSize(); i++) {
			sorted.push_back(part.get(i));
			erase(0);
		}
		part.~Flights();
	}

	flights.swap(sorted);
}



/* For multiple scheduling */


int Flights::getCompletedSize() const {

	int completed = 0;

	for (int i = 0; i < getSize(); i++) {

		if (get(i).getComplete() == true) {
			completed = completed + 1;
		}
	}

	return completed;
}

Flights Flights::rangeOf(Time window) {

	Flights splited;

	int windowSize = window.get(Time::PSEC);

	int startLine = getFirstEntryTime().get(Time::PSEC);
	int cutline = startLine + windowSize;

	for (int i = 0; i < getSize(); i++) {

		if (cutline >= get(i).getFirstEntryTime().get(Time::PSEC)) {
			splited.add(get(i));
		}
	}
	return splited;
}

void Flights::reconfigure(Time lookAhead, Links links) {

	for (int i = 0; i < getSize(); i++) {
		flights.at(i).setTempRoute(lookAhead, links);
	}
}

void Flights::update(Time cycle, Links links) {

	for (int i = 0; i < getSize(); i++) {
		if (flights.at(i).getReset() == false) {
			flights.at(i).update(cycle, links);
		}
	}
}

void Flights::mergeUpdated(Flights newFlights) {

	if (flights.empty()) {
		for (int i = 0; i < newFlights.getSize(); i++) {
			if (newFlights.get(i).getReset() == false) {
				add(newFlights.get(i));
			}
		}
	} else {

		int i = 0;
		bool exist = false;
		
		while(1) {

			if (i >= getSize()) {
				break;
			}

			for (int j = 0; j < newFlights.getSize(); j++) {

				if (get(i).equals(newFlights.get(j))) {	// the case to merge the flight itself
										
					exist = true;

					if (newFlights.get(j).getReset() == false) {
						flights.at(i).mergeUpdated(newFlights.get(j));
						i = i + 1;
					} else {
						flights.at(i).setReset(true);
						flights.at(i).setOffset(newFlights.get(j).getOffset());
					}
					newFlights.erase(j);
					
					break;
				}
			}

			if (exist == false) {
				i = i + 1;
			} else {
				exist = false;
			}
		}

		if (newFlights.getSize() > 0) {
			for (int i = 0; i < newFlights.getSize(); i++) {
				add(newFlights.get(i));
			}
		}
	}
}

void Flights::handleOverlap(Flights newFlights) {

	for (int i = 0; i < getSize(); i++) {

		for (int j = 0; j < newFlights.getSize(); j++) {

			if (get(i).equals(newFlights.get(j))) {

				if (newFlights.get(j).getReset() == false) {
					flights.at(i).handleOverlap(newFlights.get(j));
				}
				newFlights.erase(j);

				break;
			}
		}
	}
}

void Flights::handleComplete(Flights newFlights) {
	
	int i = 0;

	int pos = 0;

	while (1) {

		if (i >= getSize()) {
			break;
		}
		
		bool complete = false;
		bool reset = false;
		
		for (int j = 0; j < newFlights.getSize(); j++) {
			if (get(i).equals(newFlights.get(j))) {
				if (newFlights.get(j).getComplete() == true) {
					complete = true;
					pos = i;
				} else if (newFlights.get(j).getReset() == true) {
					flights.at(i).setOffset(newFlights.get(j).getOffset());
					flights.at(i).routeOffset();
					flights.at(i).setOffset(Time::ZERO);
				}
			}
		}

		if (complete == true) {
			erase(pos);
		} else {
			i = i + 1;
		}
	}
}

void Flights::eraseResets() {

	int i = 0;

	int pos = 0;

	while (1) {

		if (i >= getSize()) {
			break;
		}

		bool reset = false;

		if (get(i).getReset() == true) {
			reset = true;
			pos = i;
		}
		
		if (reset == true) {
			erase(pos);
		} else {
			i = i + 1;
		}
	}
}