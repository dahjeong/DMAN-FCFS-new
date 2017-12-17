#include "Nodes.h"



Nodes::Nodes() {}

void Nodes::add(Node node) {

	bool exist = false;

	if (getSize() == 0) {
		
		nodes.push_back(node);
	
	} else {
		
		for (int i = 0; i < getSize(); i++) {

			if (nodes.at(i).getId() == node.getId()) {
				exist = true;
			}
		}
		if (exist == false) {
			nodes.push_back(node);
		}
	}
}

void Nodes::erase(int i) {
	nodes.erase(nodes.begin() + i);
}

void Nodes::clear() {
	nodes.clear();
}

int Nodes::getSize() const {
	return static_cast<int>(nodes.size());
}

Node Nodes::get(int i) const {
	return nodes.at(i);
}

Node Nodes::get(string id) const {

	Node node;

	for (int i = 0; i < getSize(); i++) {

		string oldId = nodes.at(i).getId();

		if (oldId == id) {

			node = nodes.at(i);
			break;

		} else {

			if (i == getSize() - 1) {
				cout << "Node " << id << " does not exist.\n" << endl;
				system("pause");;
			}
		}
	}

	return node;
}

Nodes Nodes::getSameTypes(string type) {

	Nodes _nodes;

	for (int i = 0; i < getSize(); i++) {

		string oldType = nodes.at(i).getType();

		if (oldType == type) {

			_nodes.add(nodes.at(i));			
		}
	}

	return _nodes;
}


void Nodes::setInitialConstraints(string id, Count depRate, Count arrRate, Count passRate) {

	for (int i = 0; i < getSize(); i++) {

		string oldId = nodes.at(i).getId();

		if (oldId == id) {

			nodes.at(i).addDepRate(depRate);
			nodes.at(i).addArrRate(arrRate);
			nodes.at(i).addPassRate(passRate);
			break;

		} else {

			if (i == getSize() - 1) {
				cout << "Node " << id << " does not exist.\n" << endl;
				system("pause");;
			}
		}
	}
}

void Nodes::update(string id, Time time, int index, string state, string wakeCategory, int direction) {

	string type, threshold;
	
	for (int i = 0; i < getSize(); i++) {
		
		if (nodes.at(i).getId() == id) {

			type = nodes.at(i).getType();
			threshold = nodes.at(i).getComment();

			nodes.at(i).update(time, index, state, wakeCategory, direction);

			break;

		} else {

			if (i == getSize() - 1) {
				cout << "Node " << id << " does not exist.\n" << endl;
				system("pause");;
			}
		}
	}

	/* Runway update for opposite threshold node */
	if (type.find("RWY") != string::npos && threshold.size() > 1) {	

		int oppDirection = 0;

		if (direction == 1) {

			oppDirection = -1;

		} else if (direction == -1) {

			oppDirection = 1;
		}
		for (int j = 0; j < getSize(); j++) {

			if (nodes.at(j).getId() != id && nodes.at(j).getType() == type) {

				if (nodes.at(j).getComment() == threshold) {
					nodes.at(j).update(time, index, state, wakeCategory, oppDirection);
				} 			
			}
		}	
	}
}

void Nodes::erase(string id, Time time, int index, string state, string wakeCategory, int direction) {

	string type, threshold;

	for (int i = 0; i < getSize(); i++) {

		if (nodes.at(i).getId() == id) {

			type = nodes.at(i).getType();
			threshold = nodes.at(i).getComment();

			nodes.at(i).erase(time, index, state, wakeCategory, direction);

			break;

		} else {

			if (i == getSize() - 1) {
				cout << "Node " << id << " does not exist.\n" << endl;
				system("pause");;
			}
		}
	}

	/* Runway update for opposite threshold node */
	if (type.find("RWY") != string::npos && threshold.size() > 1) {

		int oppDirection = 0;

		if (direction == 1) {

			oppDirection = -1;

		} else if (direction == -1) {

			oppDirection = 1;
		}

		for (int j = 0; j < getSize(); j++) {

			if (nodes.at(j).getId() != id && nodes.at(j).getType() == type) {

				if (nodes.at(j).getComment() == threshold) {
					nodes.at(j).erase(time, index, state, wakeCategory, oppDirection);
				}
			}
		}
	}
}