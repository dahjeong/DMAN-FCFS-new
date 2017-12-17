/* 
	By Hyeonwoong Lee, 2017.10.31 
*/


#include "FileWrite.h"



FileWrite::FileWrite()
{
}


void FileWrite::writeLinkData(Links links, string fileName) {

	cout << "Writing updated link data....\n" << endl;

	ofstream outFile;
	outFile.open(fileName);
	outFile << "* Link capacity after scheduling" << endl;
	outFile << "* Link ID, Time (sec), Max Capacity, Count" << endl;
	string linkId;
	int time = 0;
	int MAX = 0;
	int count = 0;

	for (int i = 0; i < links.getSize(); i++) {

		Link link = links.get(i);

		for (int j = 0; j < link.getCountSize(); j++) {

			linkId = link.getId();
			time = link.getCountTime(j);
			MAX = link.getCapacity(0);
			count = link.getCount(j);

			outFile << linkId << "," << time << "," << MAX << "," << count << endl;
		}
	}
	cout << "Complete!\n" << endl;
}


void FileWrite::writeNodeData(Nodes nodes, string fileName) {

	cout << "Writing updated node data....\n" << endl;

	ofstream outFile;
	outFile.open(fileName);

	outFile << "* Currently node departure ,arrival and passing time after scheduling" << endl;
	outFile << "* Node ID, Departure Time (sec), Arrival Time (sec), Passing Time (sec)" << endl;

	string nodeId, type, comment;

	int timeMin = 0;
	int passingTime = timeMin;
	int deptime = timeMin;
	int arrtime = timeMin;

	int stripSize = 0;

	for (int i = 0; i < nodes.getSize(); i++) {

		Node node = nodes.get(i);

		nodeId = node.getId();
		type = node.getType();
		comment = node.getComment();

		stripSize = node.getStripSize();

		if (stripSize == 0) {

			outFile << nodeId << "," << timeMin << "," << timeMin << "," << timeMin << endl;

		} else {

			vector<int> depTimeList, arrTimeList, passTimeList;

			if (type == "Gate" || type.find("RWY") != string::npos) {

				for (int j = 0; j < stripSize; j++) {

					Time striptime = node.getStrip(j).getTime();
					string stripState = node.getStrip(j).getState();

					if (stripState == "DEP") {

						depTimeList.push_back(striptime.get(Time::SEC));

					} else if (stripState == "ARR") {

						arrTimeList.push_back(striptime.get(Time::SEC));

					} else {

						passTimeList.push_back(striptime.get(Time::SEC));
					}
				}
			} else {

				for (int j = 0; j < stripSize; j++) {

					Time striptime = node.getStrip(j).getTime();
					passTimeList.push_back(striptime.get(Time::SEC));
				}
			}

			int iterationSize = max(max(depTimeList.size(), arrTimeList.size()), passTimeList.size());

			for (int j = 0; j < iterationSize; j++) {

				if (depTimeList.size() <= j) {

					deptime = 0;

				} else {

					deptime = depTimeList.at(j);
				}

				if (arrTimeList.size() <= j) {

					arrtime = 0;

				} else {

					arrtime = arrTimeList.at(j);
				}

				if (passTimeList.size() <= j) {

					passingTime = 0;

				} else {

					passingTime = passTimeList.at(j);
				}

				outFile << nodeId << "," << deptime << "," << arrtime << "," << passingTime << endl;
			}
		}
	}
	cout << "Complete!\n" << endl;
}

void FileWrite::writeScheduleData(Flights flights, string fileName) {

	cout << "Writing new flight Schedule.... \n" << endl;

	ofstream outFile;
	outFile.open(fileName);

	outFile << "* Delay is in the first row of each flight." << endl;
	outFile << "* Flight ID,Address,Type,Wake Category,State,Connection,Entry Time (seconds),Exit Time (seconds),Transit Time (seconds),Previous Link,Current Link,Next Link,Speed-Up (%),Slow-Down (%),Delay (seconds)" << endl;
	
	for (int i = 0; i < flights.getSize(); i++) {

		Flight result = flights.get(i);

		string flightId = result.getId();
		string address = result.getAddress();
		string type = result.getType();
		string wake = result.getWakeCategory();
		string state = result.getState();
		string connection = result.getConnection();

		Route finalRoute = result.getNewRoute();

		string previousLink, currentLink, nextLink;
		string speedUp, slowDown;
		string delay;

		int entryTime = 0;
		int exitTime = 0;
		int transitTime = 0;

		for (int j = 0; j < finalRoute.getPathSize() - 2; j++) {

			entryTime = finalRoute.getEntryTime(j).get(Time::SEC);
			exitTime = finalRoute.getExitTime(j).get(Time::SEC);
			transitTime = finalRoute.getTransitTime(j).get(Time::SEC);

			if (j == 0) {
				previousLink = "XXXX";
				currentLink = finalRoute.getPath(j);
				nextLink = finalRoute.getPath(j + 1);
			}
			else if (j == finalRoute.getPathSize() - 3) {
				previousLink = finalRoute.getPath(j + 1);
				currentLink = finalRoute.getPath(j + 2);
				nextLink = "XXXX";
			}
			else {
				previousLink = finalRoute.getPath(j);
				currentLink = finalRoute.getPath(j + 1);
				nextLink = finalRoute.getPath(j + 2);
			}

			// Speed Changes
			speedUp = to_string(long long(finalRoute.getSpeedUp(j)));
			slowDown = to_string(long long(finalRoute.getSlowDown(j)));
						
			if (j == 0) {

				int entryTime1 = result.getNewRoute().getFirstEntryTime().get(Time::SEC);
				int entryTime2 = result.getOriginalRoute().getFirstEntryTime().get(Time::SEC);
				int delay1 = (entryTime1 - entryTime2);
				
				delay = to_string(delay1);

			} else {

				delay = "-";
			}

			outFile << flightId << "," << address << "," << type << "," << wake << "," << state << "," << connection << ","
				<< entryTime << "," << exitTime << "," << transitTime << "," << previousLink << "," << currentLink << "," << nextLink << ","
				<< speedUp << "," << slowDown  << "," << delay  << endl;
		}	
	}
	cout << "Complete!\n" << endl;
}

void FileWrite::writeDepScheduleData(Flights flights, string fileName) {

	cout << "Writing new flight Schedule for Departure.... \n" << endl;

	ofstream outFile;
	outFile.open(fileName);

	outFile << "* Delay is in the first row of each flight." << endl;
	outFile << "* Flight ID,Address,Type,Wake Category,State,Connection,Entry Time (seconds),Exit Time (seconds),Transit Time (seconds),Previous Link,Current Link,Next Link,Speed-Up (%),Slow-Down (%),Delay (seconds)" << endl;

	for (int i = 0; i < flights.getSize(); i++) {

		Flight result = flights.get(i);

		string flightId = result.getId();
		string address = result.getAddress();
		string type = result.getType();
		string wake = result.getWakeCategory();
		string state = result.getState();
		string connection = result.getConnection();

		Route finalRoute = result.getNewRoute();

		string previousLink, currentLink, nextLink;
		string speedUp, slowDown;
		string delay;

		int entryTime = 0;
		int exitTime = 0;
		int transitTime = 0;

		if (state == "DEP") {

			for (int j = 0; j < finalRoute.getPathSize() - 2; j++) {

				entryTime = finalRoute.getEntryTime(j).get(Time::SEC);
				exitTime = finalRoute.getExitTime(j).get(Time::SEC);
				transitTime = finalRoute.getTransitTime(j).get(Time::SEC);

				if (j == 0) {
					previousLink = "XXXX";
					currentLink = finalRoute.getPath(j);
					nextLink = finalRoute.getPath(j + 1);
				} else if (j == finalRoute.getPathSize() - 3) {
					previousLink = finalRoute.getPath(j + 1);
					currentLink = finalRoute.getPath(j + 2);
					nextLink = "XXXX";
				} else {
					previousLink = finalRoute.getPath(j);
					currentLink = finalRoute.getPath(j + 1);
					nextLink = finalRoute.getPath(j + 2);
				}

				// Speed Changes
				speedUp = to_string(long long(finalRoute.getSpeedUp(j)));
				slowDown = to_string(long long(finalRoute.getSlowDown(j)));

				if (j == 0) {

					int entryTime1 = result.getNewRoute().getFirstEntryTime().get(Time::SEC);
					int entryTime2 = result.getOriginalRoute().getFirstEntryTime().get(Time::SEC);
					int delay1 = (entryTime1 - entryTime2);

					delay = to_string(delay1);

				} else {

					delay = "-";
				}

				outFile << flightId << "," << address << "," << type << "," << wake << "," << state << "," << connection << ","
					<< entryTime << "," << exitTime << "," << transitTime << "," << previousLink << "," << currentLink << "," << nextLink << ","
					<< speedUp << "," << slowDown  << "," << delay  << endl;
			}
		}
	}
	cout << "Complete!\n" << endl;
}

void FileWrite::writeArrScheduleData(Flights flights, string fileName) {

	cout << "Writing new flight Schedule for Departure.... \n" << endl;

	ofstream outFile;
	outFile.open(fileName);

	outFile << "* Delay is in the first row of each flight." << endl;
	outFile << "* Flight ID,Address,Type,Wake Category,State,Connection,Entry Time (seconds),Exit Time (seconds),Transit Time (seconds),Previous Link,Current Link,Next Link,Speed-Up (%),Slow-Down (%),Delay (seconds)" << endl;

	for (int i = 0; i < flights.getSize(); i++) {

		Flight result = flights.get(i);

		string flightId = result.getId();
		string address = result.getAddress();
		string type = result.getType();
		string wake = result.getWakeCategory();
		string state = result.getState();
		string connection = result.getConnection();

		Route finalRoute = result.getNewRoute();

		string previousLink, currentLink, nextLink;
		string speedUp, slowDown;
		string delay;

		int entryTime = 0;
		int exitTime = 0;
		int transitTime = 0;

		if (state == "ARR") {

			for (int j = 0; j < finalRoute.getPathSize() - 2; j++) {

				entryTime = finalRoute.getEntryTime(j).get(Time::SEC);
				exitTime = finalRoute.getExitTime(j).get(Time::SEC);
				transitTime = finalRoute.getTransitTime(j).get(Time::SEC);

				if (j == 0) {
					previousLink = "XXXX";
					currentLink = finalRoute.getPath(j);
					nextLink = finalRoute.getPath(j + 1);
				} else if (j == finalRoute.getPathSize() - 3) {
					previousLink = finalRoute.getPath(j + 1);
					currentLink = finalRoute.getPath(j + 2);
					nextLink = "XXXX";
				} else {
					previousLink = finalRoute.getPath(j);
					currentLink = finalRoute.getPath(j + 1);
					nextLink = finalRoute.getPath(j + 2);
				}

				// Speed Changes
				speedUp = to_string(long long(finalRoute.getSpeedUp(j)));
				slowDown = to_string(long long(finalRoute.getSlowDown(j)));

				if (j == 0) {

					int entryTime1 = result.getNewRoute().getFirstEntryTime().get(Time::SEC);
					int entryTime2 = result.getOriginalRoute().getFirstEntryTime().get(Time::SEC);
					int delay1 = (entryTime1 - entryTime2);

					delay = to_string(delay1);

				} else {

					delay = "-";
				}

				outFile << flightId << "," << address << "," << type << "," << wake << "," << state << "," << connection << ","
					<< entryTime << "," << exitTime << "," << transitTime << "," << previousLink << "," << currentLink << "," << nextLink << ","
					<< speedUp << "," << slowDown  << "," << delay  << endl;
			}
		}
	}
	cout << "Complete!\n" << endl;
}