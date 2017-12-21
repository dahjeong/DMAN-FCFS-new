#include "Scheduler.h"



Scheduler::Scheduler() {}

Scheduler::Scheduler(Nodes nodes, Links links, SeparationMinima sepMinima) : nodes(nodes), links(links), sepMinima(sepMinima) {}


Flights Scheduler::first_come_first_served(const Flights& flights_original, bool multiple) {

	cout << "Start computing...\n" << endl;
	cout << "Total " << flights_original.getSize() << " flights.\n" << endl;

	chrono::steady_clock::time_point begin = chrono::steady_clock::now();

	/* Initialize */
	Flights flights_scheduled;
	
	Flight flight;

	int index = 0;
	
	string state, wakeCategory;
	Route route, newRoute;

	Node node;
	Link link;

	string temp = "_TEMP";

	string curPath, curNode, nextNode;

	bool forward = false;
	bool backward = false;

	vector<Intervals> cabinet_F, cabinet_B;
	
	Interval	entry, exit;

	Intervals	temp1, temp2, opens_node, opens_link,  
				possibles, entries, exits, buffers;

	Time	entryBegin, entryEnd, 
			exitBegin, exitEnd, 
			transit1, transit2, 
			earliestDep, earliestArr, 
			newEntry, newExit, newTransit;

	int idx = 0;
	int dir_link = 0;
	int dir_node1 = 0;	int dir_node2 = 0;	
	int speedUp = 0;	int slowDown = 0;


	/* for multiple scheduling */
	Route scheduledRoute;
	Time lastExit;		
	bool tempStart = false;


	for (int f = 0; f < flights_original.getSize(); f++) {
				
		flight = flights_original.get(f);
		cout << "\n# " << f << endl;
		flight.display();
				
		//* Main algorithm *//

		flight.setReset(false);

		if (multiple == true) {

			route = flight.getTempRoute();
			scheduledRoute = flight.getNewRoute();

			if (!scheduledRoute.empty()) {
				lastExit = scheduledRoute.getLastExitTime();
			}
		} else {
			route = flight.getOriginalRoute();
		}

		index = flight.getIndex();
		state = flight.getState();
		wakeCategory = flight.getWakeCategory();

		idx = route.getPathSize() - 1;

		/* Propagation steps */
		if (forward == false && backward == false) {		// Forward propagation

			for (int p = 0; p < route.getPathSize(); p++) {

				curPath = route.getPath(p);

				if (p == 0) {
					// start point
					if (curPath.find("TEMP") != string::npos) {

						tempStart = true;
						opens_node = Intervals::OPEN;

					} else {						
						
						node = nodes.get(curPath);

						if (node.getType().find("RWY") != string::npos) {

							temp1 = node.getAvailableSlots2(index, state, wakeCategory, sepMinima);
							temp2 = node.getAvailableSlots();
							opens_node = temp1.intersectionSets2(temp2);

						} else {

							opens_node = node.getAvailableSlots();

							if (multiple ==  true && node.selfBlock(index)) {
								Time time = node.getTimeByIdx(index);
								int rate = node.getPassRateN(time);
								int block = (int) round((double)Time(1, Time::HOUR).get(Time::PSEC)/(double)rate + 0.5);
								opens_node = opens_node.unionSet2(Interval(time.get(Time::PSEC) - block, time.get(Time::PSEC) + block));
							} 
						}
					}

					entryBegin = route.getFirstEntryTime();
					entryEnd = Time::TIMEMAX;

					possibles = opens_node.intersectionSet2(Interval(entryBegin.get(Time::PSEC), entryEnd.get(Time::PSEC)));

					cabinet_F.push_back(possibles);

					curNode = curPath;

				} else if (p == idx) {
					// end point
					if (curPath.find("TEMP") != string::npos) {

						opens_node = Intervals::OPEN;

					} else {
						
						node = nodes.get(curPath);

						if (node.getType().find("RWY") != string::npos) {

							temp1 = node.getAvailableSlots2(index, state, wakeCategory, sepMinima);
							temp2 = node.getAvailableSlots();
							
							opens_node = temp1.intersectionSets2(temp2);

						} else {

							opens_node = node.getAvailableSlots();

							if (multiple ==  true && node.selfBlock(index)) {
								Time time = node.getTimeByIdx(index);
								int rate = node.getPassRateN(time);
								int block = (int)round((double)Time(1, Time::HOUR).get(Time::PSEC)/(double)rate + 0.5);
								opens_node = opens_node.unionSet2(Interval(time.get(Time::PSEC) - block, time.get(Time::PSEC) + block));
							}
						}
					}

					possibles = opens_node.intersectionSets2(cabinet_F.back());

					earliestArr = Time(possibles.getMin(), Time::PSEC);

					cabinet_F.pop_back();
					cabinet_F.push_back(possibles);

				} else {

					// current entry node
					if (curNode.find("TEMP") != string::npos) {

						opens_node = Intervals::OPEN;

						string temp = "_TEMP";
						curNode.erase(curNode.find(temp), temp.length());

					} else {
						
						node = nodes.get(curNode);
		
						if (node.getType().find("RWY") != string::npos) {
							temp1 = node.getAvailableSlots2(index, state, wakeCategory, sepMinima);
							temp2 = node.getAvailableSlots();
							opens_node = temp1.intersectionSets2(temp2);
						} else {
							opens_node = node.getAvailableSlots();

							if (multiple ==  true && node.selfBlock(index)) {
								Time time = node.getTimeByIdx(index);
								int rate = node.getPassRateN(time);
								int block = (int)round((double)Time(1, Time::HOUR).get(Time::PSEC)/(double)rate + 0.5);
								opens_node = opens_node.unionSet2(Interval(time.get(Time::PSEC) - block, time.get(Time::PSEC) + block));
							}
						}
					}
					entries = cabinet_F.at(p - 1).intersectionSets2(opens_node);	// Intersections between initial entry slots and available slots of entry node

					// link transit
					link = links.get(curPath);

					if (link.getNode1() == curNode) {						
						if (tempStart == true && p == 1) {
							dir_link = -1;
							nextNode = link.getNode1();
						} else {
							dir_link = 1;
							nextNode = link.getNode2();
						}
					} else {						
						if (tempStart == true && p == 1) {
							dir_link = 1;
							nextNode = link.getNode2();
						} else {
							dir_link = -1;
							nextNode = link.getNode1();
						}
					}

					opens_link = link.getAvailableSlots(dir_link);		// available slots of link
										
					entries = entries.intersectionSets2(opens_link);	// intersections between entry slots and available slots of link
					
					speedUp = -route.getSpeedUp(p - 1);
					slowDown = route.getSlowDown(p - 1);
					transit1 = route.getTransitTime(p - 1).proportion(speedUp);
					transit2 = route.getTransitTime(p - 1).proportion(slowDown);
					
					exits = entries.offset(transit1.get(Time::PSEC), transit2.get(Time::PSEC));

					// link constraint operation
					for (int i = 0; i < exits.getSize(); i++) {

						buffers = opens_link.linkConstraints(entries.get(i), exits.get(i));

						for (int j = 0; j < buffers.getSize(); j++) {
							possibles.add2(buffers.get(j));
						}
					}

					cabinet_F.pop_back();	
					cabinet_F.push_back(entries);		// update forward cabinet with new entry slots
					
					cabinet_F.push_back(possibles);		// store possible exit slots into forward cabinet

					curNode = nextNode;
				}

				opens_node.clear();
				opens_link.clear();
				possibles.clear();
				entries.clear();
				exits.clear();
				buffers.clear();
			}

			forward = true;
		}
		
		if (forward == true && backward == false) {	// Backward propagation

			for (int p = 0; p < route.getPathSize(); p++) {

				curPath = route.getPath(idx - p);

				if (p == 0) {
					// end point				
					entryBegin = earliestArr;
					entryEnd = entryBegin;

					possibles.add(Interval(entryBegin.get(Time::PSEC), entryEnd.get(Time::PSEC)));

					cabinet_B.push_back(possibles);

				} else if (p == idx) {
					// start point
					earliestDep = Time(cabinet_B.back().getMin(), Time::PSEC);

				} else {

					speedUp = -route.getSpeedUp(idx - p - 1);
					slowDown = route.getSlowDown(idx - p - 1);

					transit1 = route.getTransitTime(idx - p - 1).proportion(speedUp);
					transit2 = route.getTransitTime(idx - p - 1).proportion(slowDown);

					exitBegin = entryBegin;
					exitEnd = entryEnd;
					entryBegin = exitBegin.subtract(transit2);
					entryEnd = exitEnd.subtract(transit1);
										
					exit = Interval(exitBegin.get(Time::PSEC), exitEnd.get(Time::PSEC));
					entry = Interval(entryBegin.get(Time::PSEC), entryEnd.get(Time::PSEC));

					if (p == 16) {
						int A = 0;
					}

					possibles = cabinet_F.at(idx - p - 1).intersectionSet2(entry);

					cabinet_B.push_back(possibles);

					entryBegin = Time(possibles.getFirst().getBegin(), Time::PSEC);
					entryEnd = Time(possibles.getFirst().getEnd(), Time::PSEC);
				}

				opens_node.clear();
				opens_link.clear();
				possibles.clear();
				entries.clear();
				exits.clear();
				buffers.clear();
			}
			backward = true;
		}

		if (!scheduledRoute.empty()) {		
			if (earliestDep.subtract(lastExit).get(Time::PSEC) > 0.0) {
				flight.setReset(true);
				flight.setOffset(earliestDep.subtract(lastExit));
			}
		} 
		
		// not reset
		if (forward == true && backward == true && flight.getReset() == false) {

			// new schedule
			newRoute.setId("NEW");

			idx = static_cast<int>(cabinet_B.size()) - 1;

			double speedRate = 0;
			int nom = 0;
			int den = 0;

			for (int c = 1; c < cabinet_B.size(); c++) {

				newEntry = Time(cabinet_B.at(idx - c + 1).getFirst().getBegin(), Time::PSEC);
				newExit = Time(cabinet_B.at(idx - c).getFirst().getBegin(), Time::PSEC);
				newTransit = newExit.subtract(newEntry);
				
				nom = route.getTransitTime(c - 1).subtract(newTransit).get(Time::PSEC);
				den = route.getTransitTime(c - 1).get(Time::PSEC);

				speedRate = (double) nom / (double) den;
				
				if (speedRate > 0) {	// speed-up

					speedRate = (double) floor(speedRate*100)/100;

					speedUp = (int) (speedRate*100);					
					slowDown = 0;
				
				} else if (speedRate < 0) {	// slow-down

					speedRate = (double) ceil(-speedRate*100)/100;

					speedUp = 0;
					slowDown = (int) (speedRate*100);
				
				} else {
					speedUp = 0;
					slowDown = 0;
				}

				newRoute.addTimeSet(newEntry, newExit, newTransit);
				newRoute.addSpeedRate(speedUp, slowDown);
			}

			for (int r = 0; r < route.getPathSize(); r++) {
				newRoute.addPath(route.getPath(r));
			}

			flight.setNewRoute(newRoute);

			/* update node-link model */	
			if (state == "DEP") {
				dir_node1 = 0;
				dir_node2 = 1;
			} else {
				dir_node1 = 1;
				dir_node2 = 0;
			}

			idx = newRoute.getPathSize() - 1;

			for (int k = 0; k < newRoute.getPathSize(); k++) {

				curPath = newRoute.getPath(k);

				if (k == 0) {
					// update start node 
					curNode = curPath;

					if (curPath.find("TEMP") != string::npos) {
						tempStart = true;
						curNode.erase(curNode.find(temp), temp.length());
					} else {
						nodes.update(curPath, newRoute.getFirstEntryTime(), index, state, wakeCategory, dir_node1);
					}

				} else if (k == idx) {
					// update end node 		
					if (curPath.find("TEMP") == string::npos) {
						nodes.update(curPath, newRoute.getLastExitTime(), index, state, wakeCategory, dir_node2);
					}
				} else {
					// update transit links and nodes
					link = links.get(curPath);

					if (link.getNode1() == curNode) {
						if (tempStart == true && k == 1) {
							dir_link = -1;
							nextNode = link.getNode1();
						} else {
							dir_link = 1;
							nextNode = link.getNode2();
						}
					} else {
						if (tempStart == true && k == 1) {
							dir_link = 1;
							nextNode = link.getNode2();
						} else {
							dir_link = -1;
							nextNode = link.getNode1();
						}
					}

					links.update(curPath, Interval(newRoute.getEntryTime(k - 1).get(Time::PSEC), newRoute.getExitTime(k - 1).get(Time::PSEC)), dir_link);

					if (k < idx - 1) {
						nodes.update(nextNode, newRoute.getExitTime(k - 1), index, state, wakeCategory, 0);
					}
					curNode = nextNode;
				}
			}
		}

		if (flight.getReset() == true) {	// have to be reset

			if (state == "DEP") {
				dir_node1 = 0;
				dir_node2 = 1;
			} else {
				dir_node1 = 1;
				dir_node2 = 0;
			}

			idx = scheduledRoute.getPathSize() - 1;

			for (int k = 0; k < scheduledRoute.getPathSize(); k++) {

				curPath = scheduledRoute.getPath(k);

				if (k == 0) {
					// update start node 
					curNode = curPath;

					if (curPath.find("TEMP") != string::npos) {
						tempStart = true;
						curNode.erase(curNode.find(temp), temp.length());
					} else {
						nodes.erase(curPath, scheduledRoute.getFirstEntryTime(), index, state, wakeCategory, dir_node1);
					}

				} else if (k == idx) {
					// update end node 		
					if (curPath.find("TEMP") == string::npos) {
						nodes.erase(curPath, scheduledRoute.getLastExitTime(), index, state, wakeCategory, dir_node2);
					}
				} else {
					// update transit links and nodes
					link = links.get(curPath);

					if (link.getNode1() == curNode) {
						if (tempStart == true && k == 1) {
							dir_link = -1;
							nextNode = link.getNode1();
						} else {
							dir_link = 1;
							nextNode = link.getNode2();
						}
					} else {
						if (tempStart == true && k == 1) {
							dir_link = 1;
							nextNode = link.getNode2();
						} else {
							dir_link = -1;
							nextNode = link.getNode1();
						}
					}

					links.erase(curPath, Interval(scheduledRoute.getEntryTime(k - 1).get(Time::PSEC), scheduledRoute.getExitTime(k - 1).get(Time::PSEC)), dir_link);

					if (k < idx - 1) {
						nodes.erase(nextNode, scheduledRoute.getExitTime(k - 1), index, state, wakeCategory, 0);
					}
					curNode = nextNode;
				}
			}
		}

		flights_scheduled.add(flight);

		cabinet_F.clear();
		cabinet_B.clear();

		newRoute.clear();

		scheduledRoute.clear();

		forward = false;
		backward = false;

		tempStart = false;
	}
	
	chrono::steady_clock::time_point end = chrono::steady_clock::now();

	cout << "Done.\n" << endl;
	cout << "Computation time: " << chrono::duration_cast<chrono::seconds>(end - begin).count() << " sec.\n" << endl;

	return flights_scheduled;
}


/* For multiple scheduling */
Nodes Scheduler::getNodes() {
	return nodes;
}

Links Scheduler::getLinks() {
	return links;
}

void Scheduler::setNodes(Nodes nodes) {
	this->nodes = nodes;
}

void Scheduler::setLinks(Links links) {
	this->links = links;
}


Flights Scheduler::multiple_fcfs(const Flights& flights_original, Time window, Time lookAhead, Time cycle) {

	cout << "Start computing Multiple EFCFS...\n" << endl;
	cout << "Total " << flights_original.getSize() << " flights.\n" << endl;

	chrono::steady_clock::time_point begin = chrono::steady_clock::now();

	/* Initial flights to be scheduled */
	Flights remains, part, _part, flights_scheduled;

	remains = flights_original;
	int num_flights = remains.getSize();

	/* Temporary scheduler */
	Scheduler temp(nodes, links, sepMinima);

	Nodes _nodes;	// storage for initial nodes
	Links _links;	// storage for initial links
	
	while (1) {

		if (flights_scheduled.getCompletedSize() == num_flights) {
			cout << "All flights are scheduled.\n" << endl;
			break;
		}

		_nodes = temp.getNodes();
		_links = temp.getLinks();

		remains.handleComplete(flights_scheduled);	// compare remain flights and scheduled flights

		flights_scheduled.eraseResets();
				
		part = remains.rangeOf(window);			// get flights in time window
		
		part.handleOverlap(flights_scheduled);	// handle overlapped path

		part.reconfigure(lookAhead, _links);	// routes within look ahead time

		_part = temp.first_come_first_served(part, true);	// scheduled within look ahead time
				
		// Reset node-link
		temp.setNodes(_nodes);	
		temp.setLinks(_links);	

		// Update
		_part.update(cycle, _links);	// update flights
		temp.updateNodeLink(_part);		// update node-link
		
		// Merge _part into flights_scheduled
		flights_scheduled.mergeUpdated(_part);
	}

	nodes = temp.getNodes();
	links = temp.getLinks();

	chrono::steady_clock::time_point end = chrono::steady_clock::now();

	cout << "Done.\n" << endl;
	cout << "Total computation time: " << chrono::duration_cast<chrono::seconds>(end - begin).count() << " sec.\n" << endl;

	return flights_scheduled;
}

void Scheduler::updateNodeLink(const Flights& flights_updated) {

	int index = 0; 

	string state, wakeCategory;

	string curPath, curNode, nextNode;

	string temp = "_TEMP";

	bool tempStart = false;

	int idx = 0;
	int dir_link = 0;
	int dir_node1 = 0;	int dir_node2 = 0;
	
	Flight flight;
	Route route;

	Node node;
	Link link;

	/* update node-link model */
	for (int f = 0; f < flights_updated.getSize(); f++) {

		flight = flights_updated.get(f);

		route = flight.getNewRoute();

		index = flight.getIndex();

		state = flight.getState();

		wakeCategory = flight.getWakeCategory();

		if (state == "DEP") {
			dir_node1 = 0;
			dir_node2 = 1;
		} else {
			dir_node1 = 1;
			dir_node2 = 0;
		}

		idx = route.getPathSize() - 1;

		if (flight.getReset() == false) {

			for (int k = 0; k < route.getPathSize(); k++) {

				curPath = route.getPath(k);

				if (k == 0) {
					// update start node 
					curNode = curPath;

					if (curPath.find("TEMP") != string::npos) {
						tempStart = true;
						curNode.erase(curNode.find(temp), temp.length());
					} else {
						nodes.update(curPath, route.getFirstEntryTime(), index, state, wakeCategory, dir_node1);
					}
				} else if (k == idx) {
					// update end node 		
					if (curPath.find("TEMP") == string::npos) {
						nodes.update(curPath, route.getLastExitTime(), index, state, wakeCategory, dir_node2);
					}
				} else {
					// update transit links and nodes

					link = links.get(curPath);

					if (link.getNode1() == curNode) {
						if (tempStart == true && k == 1) {
							dir_link = -1;
							nextNode = link.getNode1();
						} else {
							dir_link = 1;
							nextNode = link.getNode2();
						}
					} else {
						if (tempStart == true && k == 1) {
							dir_link = 1;
							nextNode = link.getNode2();
						} else {
							dir_link = -1;
							nextNode = link.getNode1();
						}
					}

					links.update(curPath, Interval(route.getEntryTime(k - 1).get(Time::PSEC), route.getExitTime(k - 1).get(Time::PSEC)), dir_link);

					if (k < idx - 1) {
						nodes.update(nextNode, route.getExitTime(k - 1), index, state, wakeCategory, 0);
					}
					curNode = nextNode;
				}
			}
		} else  {	// have to be reset

			for (int k = 0; k < route.getPathSize(); k++) {

				curPath = route.getPath(k);

				if (k == 0) {
					// update start node 
					curNode = curPath;

					if (curPath.find("TEMP") != string::npos) {
						tempStart = true;
						curNode.erase(curNode.find(temp), temp.length());
					} else {
						nodes.erase(curPath, route.getFirstEntryTime(), index, state, wakeCategory, dir_node1);
					}

				} else if (k == idx) {
					// update end node 		
					if (curPath.find("TEMP") == string::npos) {
						nodes.erase(curPath, route.getLastExitTime(), index, state, wakeCategory, dir_node2);
					}
				} else {
					// update transit links and nodes
					link = links.get(curPath);

					if (link.getNode1() == curNode) {
						if (tempStart == true && k == 1) {
							dir_link = -1;
							nextNode = link.getNode1();
						} else {
							dir_link = 1;
							nextNode = link.getNode2();
						}
					} else {
						if (tempStart == true && k == 1) {
							dir_link = 1;
							nextNode = link.getNode2();
						} else {
							dir_link = -1;
							nextNode = link.getNode1();
						}
					}

					links.erase(curPath, Interval(route.getEntryTime(k - 1).get(Time::PSEC), route.getExitTime(k - 1).get(Time::PSEC)), dir_link);

					if (k < idx - 1) {
						nodes.erase(nextNode, route.getExitTime(k - 1), index, state, wakeCategory, 0);
					}
					curNode = nextNode;
				}
			}
		}

		tempStart = false;
	}
}