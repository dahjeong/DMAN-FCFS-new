#include "Intervals.h"
#include "Interval.h"



const Intervals Intervals::OPEN = Intervals(Interval(Globals::MIN, Globals::MAX));	// base timeline [0, inf]

Intervals::Intervals() {}


Intervals::Intervals(Interval baseline) {	// time baseline [0, inf]
	intervals.push_back(baseline);
}

void Intervals::add(Interval newItv) {

	if (getSize() == 0) {

		intervals.push_back(newItv);

	} else {
		// open union set operation
		*this = unionSet(newItv);
	}
}

void Intervals::add2(Interval newItv) {

	if (getSize() == 0) {

		intervals.push_back(newItv);

	} else {
		// closed union set operation
		*this = unionSet2(newItv);
	}
}

void Intervals::insert(int i, Interval newItv) {
	intervals.insert(intervals.begin() + i, newItv);
}

void Intervals::set(int i, Interval newItv) {
	intervals.at(i) = newItv;
}

void Intervals::erase(int i) {
	intervals.erase(intervals.begin() + i);
}

int Intervals::getSize() const {
	return static_cast<int>(intervals.size());
}

void Intervals::clear() {
	intervals.clear();
}

void Intervals::pushBack(Interval newItv) {
	intervals.push_back(newItv);
}


vector<Interval> Intervals::getAll() const {
	return intervals;
}

Interval Intervals::get(int i) const {
	return intervals.at(i);
}

Interval Intervals::getFirst() const {
	return intervals.front();
}

Interval Intervals::getLast() const {
	return intervals.back();
}

int Intervals::getMin() const {
	return getFirst().getBegin();
}


/* Operations for Interval input */
Intervals Intervals::intersectionSet(Interval newItv) {	// open opearation for interval input

	Intervals intersections;

	Interval intersection;

	for (int i = 0; i < getSize(); i++) {

		intersection = get(i).intersectionSet(newItv);

		if (intersection.equals(Interval::NULLSET)) {

		} else {
			intersections.add(intersection);
		}
	}
	return intersections;
}

Intervals Intervals::intersectionSet2(Interval newItv) {	// closed opearation for interval input
	
	Intervals intersections;

	Interval intersection;

	for (int i = 0; i < getSize(); i++) {

		intersection = get(i).intersectionSet2(newItv);

		if (intersection.equals(Interval::NULLSET)) {
			
		} else {
			intersections.add2(intersection);
		}
	}
	return intersections;
}

Intervals Intervals::unionSet(Interval newItv) {	// open operation for interval input

	Intervals unions(*this);

	int begin = newItv.getBegin();
	int end = newItv.getEnd();

	int posBegin = 0;
	int posEnd = 0;

	int size = getSize();

	// initial position of begin
	for (int i = 0, _begin = 0; i < size; i++) {

		_begin = get(i).getBegin();

		if (begin > _begin) {
			posBegin = i + 1;
		} else if (begin == _begin) {
			posBegin = i;
		}
	}
	
	int seq = 0;
	
	bool exist = false;

	vector<int> store;

	Interval curItv, intersection, unionItv;
	
	while (seq < size) {

		curItv = intervals.at(seq);
		intersection = newItv.intersectionSet(curItv);

		if (intersection.equals(Interval::NULLSET)) {

			if (seq == size - 1 && exist == false) {
				unions.insert(posBegin, newItv);
			}
		} else {
			store.push_back(seq);
			exist = true;
		}
		seq = seq + 1;
	}

	if (!store.empty()) {

		unionItv = newItv;

		for (int s = 0; s < store.size(); s++) {
			unionItv = intervals.at(store.at(s)).unionSet(unionItv).getFirst();
		}
		for (int t = 0; t < store.size(); t++) {
			unions.erase(store.at(store.size() - t - 1));
		}
		unions.insert(store.front(), unionItv);
	}
	return unions;
}

Intervals Intervals::unionSet2(Interval newItv) {	// closed operation for interval input

	Intervals unions(*this);

	int begin = newItv.getBegin();
	int posBegin = 0;
	
	int size = getSize();

	// initial position of begin
	for (int i = 0, _begin = 0; i < size; i++) {

		_begin = get(i).getBegin();

		if (begin > _begin) {
			posBegin = i + 1;
		}
	}

	int seq = 0;

	bool exist = false;

	vector<int> store;

	Interval curItv, intersection, unionItv;

	while (seq < size) {

		curItv = intervals.at(seq);
		intersection = newItv.intersectionSet2(curItv);

		if (intersection.equals(Interval::NULLSET)) {

			if (seq == size - 1 && exist == false) {
				unions.insert(posBegin, newItv);
			}
		} else {
			store.push_back(seq);
			exist = true;
		}
		seq = seq + 1;
	}

	if (!store.empty()) {

		unionItv = newItv;

		for (int s = 0; s < store.size(); s++) {
			unionItv = intervals.at(store.at(s)).unionSet2(unionItv).getFirst();
		}
		for (int t = 0; t < store.size(); t++) {
			unions.erase(store.at(store.size() - t - 1));
		}
		unions.insert(store.front(), unionItv);
	}

	return unions;
}


/* Operations for Intervals input */
Intervals Intervals::intersectionSets(Intervals newItvs) {	// open opearation for intervals input

	Intervals intersections;

	Interval intersection;

	for (int i = 0; i < getSize(); i++) {

		Interval A = get(i);

		for (int j = 0; j < newItvs.getSize(); j++) {

			Interval B = newItvs.get(j);
			intersection = A.intersectionSet(B);

			if (intersection.equals(Interval::NULLSET)) {
				
			} else {

				intersections.add(intersection);
			}
		}
	}
	return intersections;
}

Intervals Intervals::intersectionSets2(Intervals newItvs) {	// closed opearation for intervals input

	Intervals intersections;

	Interval intersection;

	for (int i = 0; i < getSize(); i++) {

		Interval A = get(i);

		for (int j = 0; j < newItvs.getSize(); j++) {
						
			Interval B = newItvs.get(j);

			intersection = A.intersectionSet2(B);

			if (intersection.equals(Interval::NULLSET)) {

			} else {

				intersections.add2(intersection);
			}
		}
	}
	return intersections;
}

Intervals Intervals::complementarySets() {

	Intervals complementaries;

	for (int i = 0, begin = 0, end = 0; i < getSize() + 1; i++) {

		if (getSize() >= 2) {

			if (i == 0) {

				begin = Globals::MIN;
				end = get(i).getBegin();

				complementaries.add(Interval(begin, end));

			} else if (i == getSize()) {

				begin = get(i - 1).getEnd();
				end = Globals::MAX;

				complementaries.add(Interval(begin, end));

			} else {

				begin = get(i - 1).getEnd();
				end = get(i).getBegin();

				complementaries.add(Interval(begin, end));
			}
		} else {

			if (i == 0) {

				begin = Globals::MIN;
				end = get(i).getBegin();

				complementaries.add(Interval(begin, end));

				begin = get(i).getEnd();
				end = Globals::MAX;

				Interval LastSlot(begin, end);
				complementaries.add(LastSlot);
			}
		}
	}
	return complementaries;
}


/* Offset operation */
Intervals Intervals::parallelOffset(int delta) {

	Intervals offsets;

	for (int i = 0; i < getSize(); i++) {

		offsets.pushBack(get(i).parallelOffset(delta));
	}

	return offsets;
}

Intervals Intervals::offset(int delta1, int delta2) {

	Intervals offsets;

	for (int i = 0; i < getSize(); i++) {

		offsets.pushBack(get(i).offset(delta1, delta2));
	}

	return offsets;
}


/* Link constraint operation */
Intervals Intervals::linkConstraints(Interval entry, Interval exit) {	// for forward propagation

	/*	objective intervals = intervals of link = link constraints
		entry = interval of upstream node 
		exit = interval of downstream node	*/

	Intervals exits;

	int entryBegin = entry.getBegin();
	int entryEnd = entry.getEnd();

	int exitBegin = exit.getBegin();
	int exitEnd = exit.getEnd();

	int minDelta = exitBegin - entryBegin;	// minimum interval between two intervals


	for (int i = 0, begin = 0, end = 0, newExitBegin = 0, newExitEnd = 0; i < getSize(); i++) {

		// get link constraint interval
		begin = get(i).getBegin();
		end = get(i).getEnd();

		newExitBegin = Globals::NaN;
		newExitEnd = Globals::NaN;
		
		if (end - begin >= minDelta) {
			// 4 cases
			if (begin < entryBegin) {

				if (end < exitBegin) {
					// no results
				} else if (end >= exitBegin && end <= exitEnd) {

					newExitBegin = exitBegin;
					newExitEnd = end;
				
				} else {

					newExitBegin = exitBegin;
					newExitEnd = exitEnd;
				}
			} else if (begin >= entryBegin && begin <= entryEnd) {

				int delta = begin - entryBegin;

				if (end < exitBegin) {
					// no results
				} else if (end >= exitBegin && end <= exitEnd) {

					newExitBegin = exitBegin + delta;
					newExitEnd = end;

				} else {

					newExitBegin = exitBegin + delta;
					newExitEnd = exitEnd;
				}

			} else {
				// no results
			}

			if (newExitBegin != Globals::NaN && newExitEnd != Globals::NaN) {

				exits.add2(Interval(newExitBegin, newExitEnd));
			}
		} else {
			// no results
		}
	}

	return exits;
}

Intervals Intervals::linkConstraints2(Interval exit, Interval entry) {	// for backward propagation

	/*	objective intervals = intervals of link = link constraints
	entry = interval of downstream node
	exit = interval of upstream node	*/

	Intervals entries;

	int exitBegin = exit.getBegin();
	int exitEnd = exit.getEnd();

	int entryBegin = entry.getBegin();
	int entryEnd = entry.getEnd();

	
	int minDelta = exitEnd - entryBegin;	// minimum interval between two intervals

	for (int i = 0, begin = 0, end = 0, newEntryBegin = 0, newEntryEnd = 0; i < getSize(); i++) {

		// get link constraint interval
		begin = get(i).getBegin();
		end = get(i).getEnd();

		newEntryBegin = Globals::NaN;
		newEntryEnd = Globals::NaN;

		if (end - begin >= minDelta) {
			// 4 cases
			if (end > exitEnd) {

				if (begin > entryEnd) {
					// no results
				} else if (begin >= entryBegin && begin <= entryEnd) {

					newEntryBegin = begin;
					newEntryEnd = entryEnd;

				} else {

					newEntryBegin = entryBegin;
					newEntryEnd = entryEnd;
				}
			} else if (end >= exitBegin && end <= exitEnd) {

				int delta = exitEnd - end;

				if (begin > entryEnd) {
					// no results
				} else if (begin >= entryBegin && begin <= entryEnd) {

					newEntryBegin = begin;
					newEntryEnd = entryEnd - delta;

				} else {

					newEntryBegin = entryBegin;
					newEntryEnd = entryEnd - delta;
				}

			} else {
				// no results
			}

			if (newEntryBegin != Globals::NaN && newEntryEnd != Globals::NaN) {

				entries.add2(Interval(newEntryBegin, newEntryEnd));
			}
		} else {
			// no results
		}
	}

	return entries;
}


bool Intervals::exist(int n) {

	bool exist = false;

	for (int i = 0; i < getSize(); i++) {

		if (get(i).getBegin() <= n && get(i).getEnd() >= n) {

			exist = true;
			break;
		} 
	}
	return exist;
}

void Intervals::print() {

	for (int i = 0; i < getSize(); i++) {

		get(i).print();
	}
	printf("\n");
}