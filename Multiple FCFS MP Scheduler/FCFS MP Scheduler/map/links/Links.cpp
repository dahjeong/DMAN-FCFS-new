#include "Links.h"



Links::Links() {}

void Links::add(Link link) {
	bool exist = false;

	if (getSize() == 0) {

		links.push_back(link);

	} else {

		for (int i = 0; i < getSize(); i++) {

			if (links.at(i).getId() == link.getId()) {
				exist = true;
			}
		}
		if (exist == false) {
			links.push_back(link);
		}
	}
}

void Links::erase(int i) {
	links.erase(links.begin() + i);
}

void Links::clear() {
	links.clear();
}

int Links::getSize() const {
	return static_cast<int>(links.size());
}

Link Links::get(int i) const {
	return links.at(i);
}

Link Links::get(string id) const {

	Link link;

	for (int i = 0; i < getSize(); i++) {

		string oldId = links.at(i).getId();

		if (oldId == id) {

			link = links.at(i);
			break;

		} else {

			if (i == getSize() - 1) {
				cout << "Link " << id << " does not exist.\n" << endl;
				system("pause");;
			}
		}
	}

	return link;
}

void Links::setInitialConstraints(string id, Count count, Count capacity) {

	for (int i = 0; i < getSize(); i++) {

		string oldId = links.at(i).getId();

		if (oldId == id) {

			links.at(i).addCount(count);
			links.at(i).addCapacity(capacity);
			break;

		} else {

			if (i == getSize() - 1) {
				cout << "Link " << id << " does not exist.\n" << endl;
				system("pause");;
			}
		}
	}
}

void Links::setInitialAvailableSlots() {

	for (int i = 0; i < getSize(); i++) {

		links.at(i).setAvailableSlots();
	}
}

void Links::update(string id, Interval interval, int direction) {

	for (int i = 0; i < getSize(); i++) {

		string oldId = links.at(i).getId();

		if (oldId == id) {

			links.at(i).update(interval, direction);
			break;

		} else {

			if (i == getSize() - 1) {
				cout << "Link " << id << " does not exist.\n" << endl;
				system("pause");;
			}
		}
	}
}

/* for multiple scheduling */
void Links::erase(string id, Interval interval, int direction) {

	for (int i = 0; i < getSize(); i++) {

		string oldId = links.at(i).getId();

		if (oldId == id) {

			links.at(i).update(interval, -direction);
			break;

		} else {

			if (i == getSize() - 1) {
				cout << "Link " << id << " does not exist.\n" << endl;
				system("pause");;
			}
		}
	}
}
