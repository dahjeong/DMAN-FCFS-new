#pragma once

#include "..\map\nodes\Nodes.h"
#include "..\map\links\Links.h"
#include "..\flights\Flights.h"
#include "..\scheduler\separationMinima\SeparationMinima.h"

#include <vector>
#include <algorithm>

#include <string>
#include <sstream>
#include <iterator>
#include <iostream>
#include <fstream>

#include <chrono>

using namespace std;


struct State {	// for both nodes and links

	string	id;
	string	state;

	int		day;
	int		hour;
};


struct Capacity_Node {

	string	id;
	string	state;
	string	rate;

	int		depCapacity;
	int		arrCapacity;
	int		passCapacity;

	string	comment;
};

struct Capacity_Link {

	string	id;
	string	state;

	int		capacity;

	string	comment;
};



class FileRead
{
public:
	FileRead();

	/* read node-link model */
	Nodes	importNodeModel(string dataFileName, string stateFileName, string capacityFileName);
	Links	importLinkModel(string dataFileName, string stateFileName, string capacityFileName, Nodes nodes);
	
	/* node and link data */
	Nodes	readNodeData(string fileName);
	Links	readLinkData(string fileName, Nodes nodes);

	/* node and link states */
	vector<State> readStates(string fileName);	// for both nodes and links

	/* node and link capacities */
	vector<Capacity_Node> readNodeCapacity(string fileName);
	vector<Capacity_Link> readLinkCapacity(string fileName);

	/* read original flight schedule */
	Flights	importSchedule(string scheduleFileName);

	/* read separation minima */
	SeparationMinima importSepMinima(string sepMinFileName);
};

