#include "Interval.h"

const Interval Interval::NULLSET(Globals::NaN, Globals::NaN);


Interval::Interval() : begin(0), end(0) {}

Interval::Interval(int begin, int end) : begin(begin), end(end)
{	
	if (this->begin < 0 && this->begin != Globals::NaN) {
		this->begin = 0;
	}
	if (this->end < 0 && this->end != Globals::NaN) {
		this->end = 0;
	}

	if (this->begin > this->end) {
		cout << "Wrong Input: begin > end\n" << endl;
		system("pause");
	}
}


int Interval::getBegin() const {
	return begin;
}

int Interval::getEnd() const {
	return end;
}


int Interval::getSize() const {	// to get the interval size
	return end - begin;
}

bool Interval::equals(Interval newItv) {

	if (begin == newItv.getBegin() && end == newItv.getEnd()) {

		return true;

	} else {

		return false;
	}
}


Interval Interval::intersectionSet(Interval newItv) {	// intersection set operation for open interval

	// interval 1
	int begin1 = this->begin;
	int end1 = this->end;
	
	// interval 2
	int begin2 = newItv.getBegin();
	int end2 = newItv.getEnd();

	if (begin1 <= begin2 && end1 > begin2 && end1 <= end2) {

		return Interval(begin2, end1);

	} else if (begin1 <= begin2 && end1 >= end2) {

		return Interval(begin2, end2);

	} else if (begin1 >= begin2 && begin1 < end2 && end1 >= end2) {

		return Interval(begin1, end2);

	} else if (begin1 >= begin2 && end1 <= end2) {

		return Interval(begin1, end1);

	} else {

		return NULLSET;
	}
}

Interval Interval::intersectionSet2(Interval newItv) {	// intersection set operation for closed interval
	// interval 1
	int begin1 = this->begin;
	int end1 = this->end;

	// interval 2
	int begin2 = newItv.getBegin();
	int end2 = newItv.getEnd();

	if (begin1 <= begin2 && end1 > begin2 && end1 <= end2) {

		return Interval(begin2, end1);

	} else if (begin1 <= begin2 && end1 >= end2) {

		return Interval(begin2, end2);

	} else if (begin1 >= begin2 && begin1 < end2 && end1 >= end2) {

		return Interval(begin1, end2);

	} else if (begin1 >= begin2 && end1 <= end2) {
		
		return Interval(begin1, end1);

	} else if (begin1 == end2) {

		return Interval(end2, begin1);

	} else if (end1 == begin2) {

		return Interval(end1, begin2);

	} else {

		return NULLSET;
	}
}

Intervals Interval::unionSet(Interval newItv) {	// union set operation for open interval

	// interval 1
	int begin1 = this->begin;
	int end1 = this->end;

	// interval 2
	int begin2 = newItv.getBegin();
	int end2 = newItv.getEnd();

	Intervals unions;

	// open intersection set
	Interval intersection = this->intersectionSet(newItv);

	if (intersection.equals(NULLSET)) {

		if (begin1 <= begin2) {

			unions.pushBack(*this);
			unions.pushBack(newItv);

		} else {

			unions.pushBack(newItv);
			unions.pushBack(*this);
		}
		
	} else {

		if (begin1 <= begin2) {

			if (end1 <= end2) {

				unions.pushBack(Interval(begin1, end2));

			} else {

				unions.pushBack(Interval(begin1, end1));
			}
		} else {

			if (end2 <= end1) {

				unions.pushBack(Interval(begin2, end1));

			} else {

				unions.pushBack(Interval(begin2, end2));
			}
		}
	}

	return unions;
}

Intervals Interval::unionSet2(Interval newItv) {	// union set operation for closed interval

	// interval 1
	int begin1 = this->begin;
	int end1 = this->end;

	// interval 2
	int begin2 = newItv.getBegin();
	int end2 = newItv.getEnd();

	Intervals unions;

	// closed intersection set
	Interval intersection = this->intersectionSet2(newItv);

	if (intersection.equals(NULLSET)) {

		if (begin1 <= begin2) {

			unions.pushBack(*this);
			unions.pushBack(newItv);

		} else {

			unions.pushBack(newItv);
			unions.pushBack(*this);
		}

	} else {

		if (begin1 <= begin2) {

			if (end1 <= end2) {

				unions.pushBack(Interval(begin1, end2));

			} else {

				unions.pushBack(Interval(begin1, end1));
			}
		} else {

			if (end2 <= end1) {

				unions.pushBack(Interval(begin2, end1));

			} else {

				unions.pushBack(Interval(begin2, end2));
			}
		}	
	}

	return unions;
}

Intervals Interval::complementarySet() {

	Interval front(Globals::MIN, begin);
	Interval back(end, Globals::MAX);

	Intervals complementaries;
	complementaries.add(front);
	complementaries.add(back);

	return complementaries;
}

Interval Interval::parallelOffset(int delta) {
	return Interval(begin + delta, end + delta);
}

Interval Interval::offset(int delta1, int delta2) {
	return Interval(begin + delta1, end + delta2);
}

bool Interval::exist(int n) {

	if (begin <= n && end >= n) {

		return true;

	} else {

		return false;
	}
}

void Interval::print() {
	printf("{%d , %d}\n", begin, end);
}
