#pragma once

#include "..\physicalProperty\Angle.h"
#include "..\physicalProperty\Length.h"

using namespace std;

class LatLon
{
public:

	static const Length REARTH;



private:
	Angle latitude;
	Angle longitude;
	
public:
	LatLon();
	LatLon(Angle latitude, Angle longitude);

	Angle	getLatitude();
	Angle	getLongitude();

	Length	getGreatCircleDistance(LatLon latlon);

};

