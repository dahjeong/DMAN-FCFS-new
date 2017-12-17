#pragma once

#include "..\flights\Flights.h"
#include "..\map\nodes\Nodes.h"
#include "..\map\links\Links.h"
#include "separationMinima\SeparationMinima.h"

#include <vector>
#include <algorithm>
#include <chrono>
#include <iostream>

using namespace std;

class Scheduler
{
private:

	/* Map */
	Nodes	nodes;
	Links	links;

	SeparationMinima	sepMinima;

public:
	Scheduler();
	Scheduler(Nodes nodes, Links links, SeparationMinima sepMinima);

	Flights	first_come_first_served(const Flights& flights_original, bool multiple);


	/* For multiple scheduling */
	Nodes	getNodes();
	Links	getLinks();

	void	setNodes(Nodes nodes);
	void	setLinks(Links links);

	Flights	multiple_fcfs(const Flights& flights_original, Time window, Time lookAhead, Time cycle);
	
	void	updateNodeLink(const Flights& flights_updated);
};

