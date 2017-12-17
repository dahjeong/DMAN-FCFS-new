#include "SeparationMinima.h"



SeparationMinima::SeparationMinima() {}

void SeparationMinima::setAppDistance(Length appDistance) {
	this->appDistance = appDistance;
}

void SeparationMinima::addRwyOccupancy(string state, vector<string> wakeCATs, vector<int> times) {
	
	RWY_occupancy occupancy;

	occupancy.state = state;

	for (int i = 0; i < wakeCATs.size(); i++) {
		occupancy.CAT_times.push_back(CAT_Time{ wakeCATs.at(i), Time(times.at(i), Time::SEC) });
	}
	occupancies.push_back(occupancy);
}

void SeparationMinima::addSepMatrix(string id, string wakeCAT, vector<string> wakeCATs, vector<int> times) {

	Sep_Matrix matrix;
	Lead_Trails lead_trails;

	matrix.id = id;
	lead_trails.lead = wakeCAT;

	for (int i = 0; i < wakeCATs.size(); i++) {
		lead_trails.trails_CAT_times.push_back(CAT_Time{ wakeCATs.at(i), Time(times.at(i), Time::SEC) });
	}


	if (matrices.empty()) {

		matrix.leads.push_back(lead_trails);
		matrices.push_back(matrix);

	} else {

		bool exist = false;
		int pos = 0;

		for (int j = 0; j < matrices.size(); j++) {

			if (matrices.at(j).id == id) {

				exist = true;
				pos = j;
			} 			
		}
		if (exist == true) {

			matrices.at(pos).leads.push_back(lead_trails);

		} else {

			matrix.leads.push_back(lead_trails);
			matrices.push_back(matrix);
		}
	}
}


Length SeparationMinima::getAppDistance() {
	return appDistance;
}

Time SeparationMinima::getRwyOccupancyTime(string state, string wakeCategory) {

	Time ro;

	for (int i = 0; i < occupancies.size(); i++) {

		if (occupancies.at(i).state == state) {

			for (int j = 0; j < occupancies.at(i).CAT_times.size(); j++) {

				if (occupancies.at(i).CAT_times.at(j).wakeCategory == wakeCategory) {
					ro = occupancies.at(i).CAT_times.at(j).time;
					break;
				}
			}
			break;
		}
	}
	return ro;
}


Time SeparationMinima::getMinSepTime(string leadState, string trailState, string leadCAT, string trailCAT, int leadDir, int trailDir) {

	Time minSepTime;

	if (leadDir == trailDir) {

		if (leadState == "DEP"&& trailState == "DEP") {

			minSepTime = calMinSepTime_DD(leadCAT, trailCAT);

		} else if (leadState == "DEP"&& trailState == "ARR") {

			minSepTime = calMinSepTime_DA(leadCAT, trailCAT);

		} else if (leadState == "ARR"&& trailState == "DEP") {

			minSepTime = calMinSepTime_AD(leadCAT, trailCAT);

		} else if (leadState == "ARR"&& trailState == "ARR") {

			minSepTime = calMinSepTime_AA(leadCAT, trailCAT);
		}

	} else {
		
		minSepTime = calMinSepTime_OPP();
	}

	return minSepTime;
}

Time SeparationMinima::calMinSepTime_DD(string leadCAT, string trailCAT) {

	Sep_Matrix matrix;
	Time sepTime;

	for (int i = 0; i < matrices.size(); i++) {

		if (matrices.at(i).id == "DEP_DEP") {

			matrix = matrices.at(i);
			break;
		}
	}

	for (int j = 0; j < matrix.leads.size(); j++) {

		if (matrix.leads.at(j).lead == leadCAT) {

			for (int k = 0; k < matrix.leads.at(j).trails_CAT_times.size(); k++) {

				if (matrix.leads.at(j).trails_CAT_times.at(k).wakeCategory == trailCAT) {

					sepTime = matrix.leads.at(j).trails_CAT_times.at(k).time;
					break;
				}
			}
			break;
		}
	}

	return sepTime;
}

Time SeparationMinima::calMinSepTime_DA(string leadCAT, string trailCAT) {

	Sep_Matrix matrix;
	Time sepTime;

	for (int i = 0; i < matrices.size(); i++) {

		if (matrices.at(i).id == "DEP_ARR") {

			matrix = matrices.at(i);
			break;
		}
	}

	for (int j = 0; j < matrix.leads.size(); j++) {

		if (matrix.leads.at(j).lead == leadCAT) {

			for (int k = 0; k < matrix.leads.at(j).trails_CAT_times.size(); k++) {

				if (matrix.leads.at(j).trails_CAT_times.at(k).wakeCategory == trailCAT) {

					sepTime = matrix.leads.at(j).trails_CAT_times.at(k).time;
					break;
				}
			}
			break;
		}
	}

	return sepTime;
}

Time SeparationMinima::calMinSepTime_AD(string leadCAT, string trailCAT) {

	Sep_Matrix matrix;
	Time sepTime;

	for (int i = 0; i < matrices.size(); i++) {

		if (matrices.at(i).id == "ARR_DEP") {

			matrix = matrices.at(i);
			break;
		}
	}

	for (int j = 0; j < matrix.leads.size(); j++) {

		if (matrix.leads.at(j).lead == leadCAT) {

			for (int k = 0; k < matrix.leads.at(j).trails_CAT_times.size(); k++) {

				if (matrix.leads.at(j).trails_CAT_times.at(k).wakeCategory == trailCAT) {

					sepTime = matrix.leads.at(j).trails_CAT_times.at(k).time;
					break;
				}
			}
			break;
		}
	}

	return sepTime;
}

Time SeparationMinima::calMinSepTime_AA(string leadCAT, string trailCAT) {

	Sep_Matrix matrix;
	Time sepTime;

	for (int i = 0; i < matrices.size(); i++) {

		if (matrices.at(i).id == "ARR_ARR") {

			matrix = matrices.at(i);
			break;
		}
	}

	for (int j = 0; j < matrix.leads.size(); j++) {

		if (matrix.leads.at(j).lead == leadCAT) {

			for (int k = 0; k < matrix.leads.at(j).trails_CAT_times.size(); k++) {

				if (matrix.leads.at(j).trails_CAT_times.at(k).wakeCategory == trailCAT) {

					sepTime = matrix.leads.at(j).trails_CAT_times.at(k).time;
					break;
				}
			}
			break;
		}
	}

	return sepTime;
}

Time SeparationMinima::calMinSepTime_OPP() {

	return Time(2, Time::MIN);
}