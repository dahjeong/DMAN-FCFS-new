/*
	2014.08 ~ current
	By Baeseon Park, Hyeonwoong Lee, Seonyoung Kang, and Hak-Tae Lee

	* Renewal progress
	2017.09 ~ current
	By Baeseon Park, Hyeonwoong Lee, and Hak-Tae Lee

*/



#include "io\FileRead.h"
#include "io\FileWrite.h"
#include "scheduler\Scheduler.h"

void main() {

/*		
	<Renewal-Done> 
		1. This source code can run single EFCFS and multiple EFCFS.
		2. For single route only. 
		3. Based on time unit - 0.1 second. 
		4. 3 priority stretagies - Nominal, Arrival, Partial arrival priorities were added.
		5. Index column was added to the original flight schedule, which equals to the Index variable of Flight class.

	<To-Do>
		6. FileWrite class - writing function for schedule result of departure/arrival only is added.
		7. Earliest possible time concept has been changed to Earliest possible slot.
	
	<To-Do> 		
		1. The offset must be deleted in the multiple scheduling. The constraint violations in the multiple process are allowed.
		2. Dijkstra class for Route assignment
		3. Change import file format -> flight knows only scheduled entry time and gate id.
		4. Runway assignment
*/


	FileRead fr;
	
	Nodes nodes = fr.importNodeModel("RKSI_model\\RKSI_nodeData.csv", "RKSI_model\\RKSI_nodeStates.csv", "RKSI_model\\RKSI_nodeCapacities.csv");
	Links links = fr.importLinkModel("RKSI_model\\RKSI_linkData.csv", "RKSI_model\\RKSI_linkStates.csv", "RKSI_model\\RKSI_linkCapacities.csv", nodes);
	SeparationMinima sepMinima = fr.importSepMinima("RKSI_model\\SeparationMinima.csv");
	
	Flights flights_original = fr.importSchedule("RKSI_model\\RKSI_schedule_150410.csv");

	flights_original.sort_Priority_Nominal();

	Scheduler scheduler(nodes, links, sepMinima);

	Flights flights_scheduled = scheduler.first_come_first_served(flights_original, false);

	
	/*Time window(1, Time::HOUR);
	Time lookAhead(30, Time::MIN);
	Time cycle(5, Time::MIN);
	Flights flights_scheduled = scheduler.multiple_fcfs(flights_original, window, lookAhead, cycle);*/
		
	// write
	FileWrite fw;

	fw.writeScheduleData(flights_scheduled, "RKSI_New_Schedule_150410.csv");

	fw.writeDepScheduleData(flights_scheduled, "RKSI_New_Schedule_DEP_150410.csv");
	fw.writeArrScheduleData(flights_scheduled, "RKSI_New_Schedule_ARR_150410.csv");

	fw.writeNodeData(scheduler.getNodes(), "RKSI_New_Nodes_150410.csv");
	fw.writeLinkData(scheduler.getLinks(), "RKSI_New_Links_150410.csv");

	system("pause");
}