#include "FileRead.h"

/* To split strings */
template <typename Out>
void Split(const string &line, char delimiter, Out splited) {
	
	stringstream dline;
	dline.str(line);

	string item;

	while (getline(dline, item, delimiter)) {
		*(splited++) = item;
	}
}
vector<string> split(const string &line, char delimiter) {

	vector<string> splited = vector<string>();
	Split(line, delimiter, back_inserter(splited));

	return splited;
}



/* File read */
FileRead::FileRead() {}

Nodes FileRead::importNodeModel(string dataFileName, string stateFileName, string capacityFileName) {

	cout << "\nImport node data...\n" << endl;

	chrono::steady_clock::time_point begin = chrono::steady_clock::now();

	Nodes nodes = readNodeData(dataFileName);
	vector<State> states = readStates(stateFileName);
	vector<Capacity_Node> capacities = readNodeCapacity(capacityFileName);

	/* matching */
	for (int n = 0; n < nodes.getSize(); n++) {

		Node node = nodes.get(n);
		string id = node.getId();

		for (int s = 0; s < (states.size()); s++) {

			string id_s = states.at(s).id;

			if (id == id_s) {

				int day = states.at(s).day;
				int hour = states.at(s).hour;
				Time t((day - 1)*60*60*24 + hour*60*60, Time::SEC);

				string state_s = states.at(s).state;

				for (int c = 0; c < (capacities.size()); c++) {
				
					string id_c = capacities.at(c).id;
					
					if (id_s == id_c) {

						string state_c = capacities.at(c).state;

						if (state_s == state_c) {

							nodes.setInitialConstraints(id, Count(t, capacities.at(c).depCapacity), Count(t, capacities.at(c).arrCapacity), Count(t, capacities.at(c).passCapacity));
							break;
						}
					}
				}
			}
		}
	}

	cout << "Total " << nodes.getSize() << " Nodes.\n" << endl;

	chrono::steady_clock::time_point end = chrono::steady_clock::now();

	cout << "Import time: " << chrono::duration_cast<chrono::seconds>(end - begin).count() << " sec.\n" << endl;

	return nodes;
}

Links FileRead::importLinkModel(string dataFileName, string stateFileName, string capacityFileName, Nodes nodes) {

	cout << "\nImport link data...\n" << endl;

	chrono::steady_clock::time_point begin = chrono::steady_clock::now();
	
	Links links = readLinkData(dataFileName, nodes);
	vector<State> states = readStates(stateFileName);
	vector<Capacity_Link> capacities = readLinkCapacity(capacityFileName);

	/* matching */
	for (int l = 0; l < links.getSize(); l++) {

		Link link = links.get(l);
		string id = link.getId();

		for (int s = 0; s < (states.size()); s++) {

			string id_s = states.at(s).id;

			if (id == id_s) {

				int day = states.at(s).day;
				int hour = states.at(s).hour;
				Time t((day - 1)*60*60*24 + hour*60*60, Time::SEC);

				string state_s = states.at(s).state;

				for (int c = 0; c < (capacities.size()); c++) {

					string id_c = capacities.at(c).id;

					if (id_s == id_c) {

						string state_c = capacities.at(c).state;

						if (state_s == state_c) {
							links.setInitialConstraints(id, Count(t, 0), Count(t, capacities.at(c).capacity));
							break;
						}
					}
				}
			}
		}	
	}
	links.setInitialAvailableSlots();

	cout << "Total " << links.getSize() << " Links.\n" << endl;

	chrono::steady_clock::time_point end = chrono::steady_clock::now();

	cout << "Import time: " << chrono::duration_cast<chrono::seconds>(end - begin).count() << " sec.\n" << endl;

	return links;
}
 

/* informations */
Nodes FileRead::readNodeData(string fileName) {

	Nodes nodes;

	ifstream file(fileName);

	string line;

	vector<string> splited;

	while (!file.eof()) {

		getline(file, line);
		
		if (!empty(line)) {

			if (line.find("*") == string::npos) {

				splited = split(line, ',');

				string type = splited.at(0);
				string id = splited.at(1);
				Angle latitude(stod(splited.at(2)), Angle::DEG);
				Angle longitude(stod(splited.at(3)), Angle::DEG);
				string comment = splited.at(4);

				Node node(id);
				node.setType(type);
				node.setLatLon(LatLon(latitude, longitude));
				node.setComment(comment);

				nodes.add(node);
			}
		}
	}

	file.close();

	return nodes;
}

Links FileRead::readLinkData(string fileName, Nodes nodes) {

	Links links;

	ifstream file(fileName);

	string line;

	vector<string> splited;

	while (!file.eof()) {

		getline(file, line);

		if (!empty(line)) {

			if (line.find("*") == string::npos) {

				splited = split(line, ',');

				string type = splited.at(0);
				string id = splited.at(1);
				string node1 = splited.at(2);
				string node2 = splited.at(3);
				
				LatLon p1 = nodes.get(node1).getLatLon();
				LatLon p2 = nodes.get(node2).getLatLon();
				Length length = p1.getGreatCircleDistance(p2);

				Link link(id);
				link.setType(type);
				link.setNodes(node1, node2);
				link.setLength(length);

				links.add(link);
			}
		}
	}

	file.close();

	return links;
}

/* state data */
vector<State> FileRead::readStates(string fileName) {

	vector<State> states;

	ifstream file(fileName);

	string line;

	vector<string> splited;

	while (!file.eof()) {

		getline(file, line);

		if (!empty(line)) {

			if (line.find("*") == string::npos) {

				splited = split(line, ',');
								
				State state;
				state.id = splited.at(0);
				state.day = stoi(splited.at(1));
				state.hour = stoi(splited.at(2));
				state.state = splited.at(3);

				states.push_back(state);
			}
		}
	}

	file.close();
	
	return states;
}

/* capacity data */
vector<Capacity_Node> FileRead::readNodeCapacity(string fileName) {

	vector<Capacity_Node> capacities;

	ifstream file(fileName);

	string line;

	vector<string> splited;

	while (!file.eof()) {

		getline(file, line);

		if (!empty(line)) {

			if (line.find("*") == string::npos) {

				splited = split(line, ',');

				Capacity_Node capacityInfo;
				capacityInfo.id = splited.at(0);				
				capacityInfo.state = splited.at(1);
				capacityInfo.rate = splited.at(2);
				capacityInfo.depCapacity = stoi(splited.at(3));
				capacityInfo.arrCapacity = stoi(splited.at(4));
				capacityInfo.passCapacity = stoi(splited.at(5));
				capacityInfo.comment = splited.at(6);

				capacities.push_back(capacityInfo);
			}
		}
	}

	file.close();

	return capacities;
}

vector<Capacity_Link> FileRead::readLinkCapacity(string fileName) {

	vector<Capacity_Link> capacities;

	ifstream file(fileName);

	string line;

	vector<string> splited;

	while (!file.eof()) {

		getline(file, line);

		if (!empty(line)) {

			if (line.find("*") == string::npos) {

				splited = split(line, ',');

				Capacity_Link capacityInfo;
				capacityInfo.id = splited.at(0);
				capacityInfo.state = splited.at(1);				
				capacityInfo.capacity = stoi(splited.at(2));
				capacityInfo.comment = splited.at(3);

				capacities.push_back(capacityInfo);
			}
		}
	}

	file.close();

	return capacities;
}


Flights FileRead::importSchedule(string scheduleFileName) {

	chrono::steady_clock::time_point begin = chrono::steady_clock::now();


	Flights flights;

	cout << "\nImport original schedule...\n" << endl;

	ifstream file(scheduleFileName);

	string line;

	vector<vector<vector<string>>> set_3d;
	vector<vector<string>> set_2d;
	vector<string> splited;

	while (!file.eof()) {

		getline(file, line);

		if (!empty(line)) {

			if (line.find("*") == string::npos) {

				splited = split(line, ',');
				
				string id = splited.at(0);

				if (set_2d.empty()) {

					set_2d.push_back(splited);
				
				} else {

					if (id == set_2d.back().at(0)) {

						set_2d.push_back(splited);

					} else {

						set_3d.push_back(set_2d);

						set_2d.clear();

						set_2d.push_back(splited);
					}
				}
			}
		}

		if (file.eof()) {

			set_3d.push_back(set_2d);
		}
	}


	file.close();
	

	for (int i = 0; i < set_3d.size(); i++) {
		
		set_2d = set_3d.at(i);

		Route route("Route0");

		if (i == set_3d.size() - 1) {
			int A = 0;
		}

		for (int j = 0; j < set_2d.size(); j++) {

			splited = set_2d.at(j);

			Time entryTime(stoi(splited.at(7)), Time::SEC);
			Time exitTime(stoi(splited.at(8)), Time::SEC);
			Time transitTime(stoi(splited.at(9)), Time::SEC);

			string prevPath = splited.at(10);
			string curPath = splited.at(11);
			string nextPath = splited.at(12);

			int speedUp = stoi(splited.at(13));
			int slowDown = stoi(splited.at(14));

			route.addTimeSet(entryTime, exitTime, transitTime);
			route.addPath(prevPath);
			route.addPath(curPath);
			route.addPath(nextPath);
			route.addSpeedRate(speedUp, slowDown);

			if (j == (set_2d.size() - 1)) {

				int index = stoi(splited.at(0));
				string id = splited.at(1);
				string address = splited.at(2);
				string type = splited.at(3);
				string wakeCategory = splited.at(4);			
				string state = splited.at(5);
				string connection = splited.at(6);
				
				Flight flight(index, id, address, type, wakeCategory, state, connection);
				flight.setOriginalRoute(route);

				flights.add(flight);
			}
		}
	}


	chrono::steady_clock::time_point end = chrono::steady_clock::now();

	cout << "Import time: " << chrono::duration_cast<chrono::seconds>(end - begin).count() << " sec.\n" << endl;
	
	return flights;
}


SeparationMinima FileRead::importSepMinima(string sepMinFileName) {

	chrono::steady_clock::time_point begin = chrono::steady_clock::now();


	SeparationMinima sepMinima;

	cout << "\nImport separation minima...\n" << endl;

	ifstream file(sepMinFileName);

	string line, id, wakeCAT;
	
	vector<string>	splited, wakeCATs;
	vector<int>		times;

	int n = 1;
	int k = 0;

	while (!file.eof()) {

		getline(file, line);

		if (!empty(line)) {

			if (line.find("*") == string::npos) {

				splited = split(line, ',');

				if (n == 2) {
					// final approach distance to runway
					sepMinima.setAppDistance(Length(stod(splited.at(0)), Length::NMI));
				
				} else if (n == 4) {
					// wake categories
					wakeCATs.push_back(splited.at(0));
					wakeCATs.push_back(splited.at(1));
					wakeCATs.push_back(splited.at(2));
					wakeCATs.push_back(splited.at(3));

				} else if (n > 4 && n < 7) {
					// runway occupancy times
					id = splited.at(0);
					
					times.push_back(stoi(splited.at(1)));
					times.push_back(stoi(splited.at(2)));
					times.push_back(stoi(splited.at(3)));
					times.push_back(stoi(splited.at(4)));

					sepMinima.addRwyOccupancy(id, wakeCATs, times);

				} else if (n == 8 + k*6) {

					id = splited.at(0).erase(0,1);

				} else if (n == 9 + k*6) {
					
					wakeCATs.clear();
					wakeCATs.push_back(splited.at(1));
					wakeCATs.push_back(splited.at(2));
					wakeCATs.push_back(splited.at(3));
					wakeCATs.push_back(splited.at(4));

				} else if (n  > 9 + k*6 && n < 8 + (k + 1)*6) {

					wakeCAT = splited.at(0);

					times.clear();
					times.push_back(stoi(splited.at(1)));
					times.push_back(stoi(splited.at(2)));
					times.push_back(stoi(splited.at(3)));
					times.push_back(stoi(splited.at(4)));

					sepMinima.addSepMatrix(id, wakeCAT, wakeCATs, times);

					if (n == 7 + (k + 1)*6) {
						k = k + 1;
					}
				}
			}
		}

		n = n + 1;		
	}

	file.close();

	chrono::steady_clock::time_point end = chrono::steady_clock::now();

	cout << "Import time: " << chrono::duration_cast<chrono::seconds>(end - begin).count() << " sec.\n" << endl;

	return sepMinima;
}