#include "CountList.h"



CountList::CountList() { }


void CountList::add(Count count) {
	
	int time = count.getTime().get(Time::PSEC);
	int pluscount = count.getCount();

	if (countList.size() >= 1) {

		for (int i = 0, time_i = 0, oldcount = 0; i < countList.size(); i++) {

			time_i = countList.at(i).getTime().get(Time::PSEC);

			if (time_i > time) {

				countList.insert(countList.begin() + i, count);
				break;

			} else if (time_i == time) {

				oldcount = countList.at(i).getCount();
				countList.at(i).setCount(oldcount + pluscount);
				break;
			}

			if (i == countList.size() - 1) {

				countList.push_back(count);
				break;
			}
		}
	} else {

		countList.push_back(count);
	}
}

int CountList::getSize() const {
	return static_cast<int>(countList.size());
}

void CountList::clear() {
	countList.clear();
}

Count CountList::get(int i) const {
	return countList.at(i);
}

Count CountList::getFirst() const {
	return countList.front();
}

Count CountList::getLast() const {
	return countList.back();
}

int CountList::getCountN(int i) const {
	return countList.at(i).getCount();
}

int CountList::getCountN(Time time) const {

	int count = 0;

	for (int i = 0; i < getSize(); i++) {

		Count curCount = countList.at(i);
		Time oldTime = curCount.getTime();

		if (time.get(Time::PSEC) < oldTime.get(Time::PSEC)) {

			count = curCount.getCount();
			break;

		} else {

			if (i == countList.size() - 1) {

				count = curCount.getCount();
			}
		}
	}

	return count;
}

int CountList::getFirstCountN() const {
	return countList.front().getCount();
}

int CountList::getLastCountN() const {
	return countList.back().getCount();
}


CountList CountList::subtract(CountList target) {

	CountList result;

	int time_r = 0;
	int count_r = 0;

	int objectSize = getSize();
	int targetSize = target.getSize();

	// initial count values
	int objectCount = get(0).getCount();
	int targetCount = target.get(0).getCount();

	int dummy = INT_MIN;
	
	vector<vector<int>> table;
	table.resize(4);

	// store times of main countlist at the first colomn of table (table[0])
	for (int i = 0; i < objectSize; i++) {
		table[0].push_back(this->get(i).getTime().get(Time::PSEC));
	}

	// store times of object countlist at the first colomn of table (table[0])
	for (int i = 0; i < targetSize; i++) {
		table[0].push_back(target.get(i).getTime().get(Time::PSEC));
	}

	// sort the first colomn of table by time
	for (int i = 0; i < table[0].size(); i++) {
		sort(table[0].begin(), table[0].end());
	}

	// erase overlapped times
	int n = 0;

	while (1) {

		if (n >= table[0].size() - 1) {
			break;
		}
		if (table[0][n] == table[0][n + 1]) {

			table[0].erase(table[0].begin() + n);
		}
		n++;
	}

	int flag = 0;
	int index = 0;

	// store counts of main countlist to the second column of table (table[1])
	for (int i = 0; i < table[0].size(); i++) {

		for (int j = 0; j < objectSize; j++) {

			if (table[0][i] == get(j).getTime().get(Time::PSEC)) {

				flag = 1;
				index = j;
				break;

			} else {

				flag = 0;
			}
		}
		if (flag == 1) {

			table[1].push_back(get(index).getCount());
			flag = 0;

		} else {

			table[1].push_back(dummy);
		}
	}

	// store counts of main countlist to the third column of table (table[2])
	for (int i = 0; i < table[0].size(); i++) {

		for (int j = 0; j < targetSize; j++) {

			if (table[0][i] == target.get(j).getTime().get(Time::PSEC)) {

				flag = 1;
				index = j;
				break;

			} else {

				flag = 0;
			}
		}
		if (flag == 1) {

			table[2].push_back(target.get(index).getCount());
			flag = 0;

		} else {

			table[2].push_back(dummy);
		}
	}

	// convert dummies to zero
	for (int i = 0; i < table.at(0).size(); i++) {

		if (table[1][i] == dummy && i == 0) {

			table[1][i] = 0;

		} else if (table[1][i] == dummy) {

			table[1][i] = table[1][i - 1];

		} else {

			table[1][i] = table[1][i];
		}
	}
	for (int i = 0; i < table.at(0).size(); i++) {

		if (table[2][i] == dummy && i == 0) {

			table[2][i] = 0;

		} else if (table[2][i] == dummy) {

			table[2][i] = table[2][i - 1];

		} else {

			table[2][i] = table[2][i];
		}
	}

	// main - object (table[1] = table[2])
	for (int i = 0; i < table[0].size(); i++) {

		table[3].push_back(table[1][i] - table[2][i]);
	}

	// create result countlist
	for (int i = 0; i < table.at(0).size(); i++) {
				
		if (table[3][i] != 0 && i == 0) {

			time_r = table[0][i];
			count_r = table[3][i];

			Count newCount(Time(time_r, Time::PSEC), count_r);
			result.add(newCount);

		} else if (table[3][i] == 0 && i == 0) {
			
		} else {

			if (table[3][i] != table[3][i - 1]) {

				time_r = table[0][i];
				count_r = table[3][i];

				Count newCount(Time(time_r, Time::PSEC), count_r);
				result.add(newCount);
			}
		}
	}
	return result;
}


CountList CountList::add(CountList target) {

	CountList result;

	int time_r = 0;
	int count_r = 0;

	int objectSize = getSize();
	int targetSize = target.getSize();

	// initial count values
	int objectCount = get(0).getCount();
	int targetCount = target.get(0).getCount();

	int dummy = INT_MIN;


	vector<vector<int>> table;
	table.resize(4);

	for (int i = 0; i < objectSize; i++) {

		table[0].push_back(this->get(i).getTime().get(Time::PSEC));
	}
	for (int i = 0; i < targetSize; i++) {
		table[0].push_back(target.get(i).getTime().get(Time::PSEC));
	}
	for (int i = 0; i < table[0].size(); i++) {
		sort(table[0].begin(), table[0].end());
	}

	int n = 0;

	while (1) {
		if (n >= table.size() - 1) {
			break;
		}
		if (table[0][n] == table[0][n + 1]) {
			table[0].erase(table[0].begin() + n);
		}
		n++;
	}

	int flag = 0;
	int index = 0;
	
	for (int i = 0; i < table[0].size(); i++) {

		for (int j = 0; j < objectSize; j++) {

			if (table[0][i] == get(j).getTime().get(Time::PSEC)) {

				flag = 1;
				index = j;
				break;

			} else {

				flag = 0;
			}
		}
		if (flag == 1) {

			table[1].push_back(get(index).getCount());
			flag = 0;

		} else {

			table[1].push_back(dummy);
		}
	}

	for (int i = 0; i < table[0].size(); i++) {

		for (int j = 0; j < targetSize; j++) {

			if (table[0][i] == target.get(j).getTime().get(Time::PSEC)) {

				flag = 1;
				index = j;
				break;

			} else {

				flag = 0;
			}
		}
		if (flag == 1) {

			table[2].push_back(target.get(index).getCount());
			flag = 0;

		} else {

			table[2].push_back(dummy);
		}
	}

	for (int i = 0; i < table.at(0).size(); i++) {

		if (table[1][i] == dummy && i == 0) {

			table[1][i] = 0;

		} else if (table[1][i] == dummy) {

			table[1][i] = table[1][i - 1];

		} else {

			table[1][i] = table[1][i];
		}
	}

	for (int i = 0; i < table.at(0).size(); i++) {

		if (table[2][i] == dummy && i == 0) {

			table[2][i] = 0;

		} else if (table[2][i] == dummy) {

			table[2][i] = table[2][i - 1];

		} else {

			table[2][i] = table[2][i];
		}
	}

	for (int i = 0; i < table[0].size(); i++) {

		table[3].push_back(table[1][i] + table[2][i]);
	}

	for (int i = 0; i < table.at(0).size(); i++) {

		if (table[3][i] != 0 && i == 0) {

			time_r = table[0][i];
			count_r = table[3][i];

			Count newCount(Time(time_r, Time::PSEC), count_r);
			result.add(newCount);

		} else if (table[3][i] == 0 && i == 0) {
			
		} else {

			if (table[3][i] != table[3][i - 1]) {

				time_r = table[0][i];
				count_r = table[3][i];

				Count newCount(Time(time_r, Time::PSEC), count_r);
				result.add(newCount);
			}
		}
	}
	return result;
}

CountList CountList::reverse() {

	CountList reverse;

	for (int i = 0; i < countList.size(); i++) {

		reverse.add(Count(countList.at(i).getTime(), -countList.at(i).getCount()));
	}

	return reverse;
}


Intervals CountList::toIntervals(int maxCount, int direction) {

	Intervals intervals;

	vector<Time> bufferTime;
	vector<int> marks;

	int mark = 0;
	
	if (direction == 1) {	// positive count

		for (int i = 0; i < getSize(); i++) {

			Time time = get(i).getTime();
			int count = get(i).getCount();

			if (0 < count && count <= maxCount) {

				mark = 1;

			} else {

				mark = 2;
			}

			bufferTime.push_back(time);
			marks.push_back(mark);
		}

		bufferTime.push_back(Time::TIMEMAX);
		marks.push_back(mark);

		for (int j = 1, now = 0, before = 0; j < bufferTime.size(); j = j + 1) {

			now = marks.at(j);
			before = marks.at(j - 1);

			if (before == 2) {

			} else {

				intervals.add2(Interval(bufferTime[j - 1].get(Time::PSEC), bufferTime[j].get(Time::PSEC)));
			}
		}

	} else {	// negative count

		for (int i = 0; i < getSize(); i++) {

			Time time = get(i).getTime();
			int count = get(i).getCount();


			if (-maxCount <= count && count < 0) {

				mark = 1;

			} else {

				mark = 2;
			}

			bufferTime.push_back(time);
			marks.push_back(mark);
		}

		bufferTime.push_back(Time::TIMEMAX);
		marks.push_back(mark);

		for (int j = 1, now = 0, before = 0; j < bufferTime.size(); j = j + 1) {

			now = marks.at(j);
			before = marks.at(j - 1);

			if (before == 2) {

			} else {

				intervals.add2(Interval(bufferTime[j - 1].get(Time::PSEC), bufferTime[j].get(Time::PSEC)));
			}
		}
	}
	return intervals;
}

Intervals CountList::toIntervals(int maxCount) {

	Intervals intervals;

	vector<Time> bufferTime;
	vector<int> marks;

	int mark = 0;

	for (int i = 0; i < getSize(); i++) {

		Time time = get(i).getTime();
		int count = get(i).getCount();

		if (0 < count && count <= maxCount) {

			mark = 1;

		} else {

			mark = 2;
		}

		bufferTime.push_back(time);
		marks.push_back(mark);
	}

	bufferTime.push_back(Time::TIMEMAX);
	marks.push_back(mark);

	for (int j = 1, now = 0, before = 0; j < bufferTime.size(); j = j + 1) {

		now = marks.at(j);
		before = marks.at(j - 1);

		if (before == 2) {

		} else {

			intervals.add2(Interval(bufferTime[j - 1].get(Time::PSEC), bufferTime[j].get(Time::PSEC)));
		}
	}

	return intervals;
}

void CountList::update(Interval interval, int direction) {

	int size = getSize();

	int begin = interval.getBegin();
	int end = interval.getEnd();

	int i = 0;
	int j = 0;

	int oldcount = 0;

	int loop1 = 0;
	int loop2 = 0;

	int time = 0;
	int count = 0;
	
	if (direction == 1) {	// same direction -> positive count

		count = 1;

	} else {				// opposite direction -> negative count

		count = -1;
	}

	if (size >= 1) {
		// begin 배치
		for (i = 0; i < getSize(); i++) {

			time = get(i).getTime().get(Time::PSEC);

			if (i == 0) {

				oldcount = 0;

			} else {

				if (time > begin) {

					oldcount = get(i - 1).getCount();

				} else if (time == begin) {

					oldcount = get(i).getCount();
				}
			}
			if (time > begin) {

				add(Count(Time(begin, Time::PSEC), oldcount + count));

				i = i + 1;

				break;

			} else if (time == begin) {

				oldcount = get(i).getCount();
				countList.at(i).setCount(oldcount + count);

				i = i + 1;

				break;

			} else {

				if (i == getSize() - 1) {

					oldcount = get(i).getCount();

					add(Count(Time(begin, Time::PSEC), oldcount + count));
					
					i = i + 1;
					loop1 = 1;

					break;
				}
			}
		}

		if (loop1 != 1) {

			for (i; i < getSize(); i++) {

				countList.at(i).setCount(get(i).getCount() + count);
			}
		}
		// exitTime 배치
		for (j = 0; j < getSize(); j++) {

			time = get(j).getTime().get(Time::PSEC);

			if (j == 0) {

				oldcount = 0;

			} else {

				if (time > end) {

					oldcount = get(j - 1).getCount();

				} else if (time == end) {

					oldcount = get(j).getCount();
				}
			}
			if (time > end) {

				add(Count(Time(end, Time::PSEC), oldcount - count));

				j = j + 1;

				break;

			} else if (time == end) {

				oldcount = get(j).getCount();
				countList.at(j).setCount(oldcount - count);

				j = j + 1;

				break;

			} else {

				if (j == getSize() - 1) {

					oldcount = get(j).getCount();

					add(Count(Time(end, Time::PSEC), oldcount - count));

					j = j + 1;
					loop2 = 2;

					break;
				}
			}
		}

		if (loop2 != 2) {

			for (j; j < getSize(); j++) {

				countList.at(j).setCount(get(j).getCount() - count);
			}
		}
	} else {

		add(Count(Time(begin, Time::PSEC), count));
		add(Count(Time(end, Time::PSEC), 0));
	}
}



void CountList::print() {

	for (int i = 0; i < getSize(); i++) {
		get(i).print();
	}
	printf("\n");
}