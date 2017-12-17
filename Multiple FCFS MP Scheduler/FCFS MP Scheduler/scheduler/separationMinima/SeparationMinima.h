#pragma once

#include "..\..\physicalProperty\Time.h"
#include "..\..\physicalProperty\Length.h"

#include <vector>
#include <algorithm>
#include <string>

using namespace std;

class SeparationMinima
{
	
private:
	/* structures */
	struct CAT_Time {
		string	wakeCategory;
		Time	time;
	};

	struct Lead_Trails {

		string				lead;
		vector<CAT_Time>	trails_CAT_times;
	};

	struct Sep_Matrix {

		string	id;
		vector<Lead_Trails>	leads;
	};

	struct RWY_occupancy {

		string				state;
		vector<CAT_Time>	CAT_times;
	};


private:
	/* private members */
	Length					appDistance;
	vector<RWY_occupancy>	occupancies;
	vector<Sep_Matrix>		matrices;
	
	
public:
	SeparationMinima();
	
	/* Initialize */
	void	setAppDistance(Length appDistance);

	void	addRwyOccupancy(string state, vector<string> wakeCATs, vector<int> times);

	void	addSepMatrix(string id, string wakeCAT, vector<string> wakeCATs, vector<int> times);

	/* accessors */
	
	Length	getAppDistance();
	Time	getRwyOccupancyTime(string state, string wakeCategory);
	

	Time	getMinSepTime(string leadState, string trailState, string leadCAT, string trailCAT, int leadDir, int trailDir);
	
	Time	calMinSepTime_DD(string leadCAT, string trailCAT);
	Time	calMinSepTime_DA(string leadCAT, string trailCAT);
	Time	calMinSepTime_AD(string leadCAT, string trailCAT);	
	Time	calMinSepTime_AA(string leadCAT, string trailCAT);

	Time	calMinSepTime_OPP();
	
};

