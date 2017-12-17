#pragma once

#include "..\map\links\Links.h"
#include "..\map\nodes\Nodes.h"
#include "..\scheduler\Scheduler.h"

#include <stdio.h>
#include <fstream>

using namespace std;

class FileWrite
{
public:
	FileWrite();

	void	writeLinkData(Links links, string fileName);
	void	writeNodeData(Nodes nodes, string fileName);
	void	writeScheduleData(Flights flights, string fileName);
	void	writeDepScheduleData(Flights flights, string fileName);
	void	writeArrScheduleData(Flights flights, string fileName);
};

