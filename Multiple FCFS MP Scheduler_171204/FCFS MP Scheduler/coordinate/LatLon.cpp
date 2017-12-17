#include "LatLon.h"



const Length LatLon::REARTH(6400.0*1000.0, Length::M);


LatLon::LatLon() { }

LatLon::LatLon(Angle latitude, Angle longitude) :latitude(latitude), longitude(longitude) {

}

Angle LatLon::getLatitude() {
	return latitude;
}

Angle LatLon::getLongitude() {
	return longitude;
}

Length LatLon::getGreatCircleDistance(LatLon latlon) {

	double phi1 = this->getLatitude().get(Angle::RAD);
	double lamda1 = this->getLongitude().get(Angle::RAD);

	double phi2 = latlon.getLatitude().get(Angle::RAD);
	double lamda2 = latlon.getLongitude().get(Angle::RAD);

	double cosTheta = cos(phi1)*cos(phi2)*cos(lamda1 - lamda2) + sin(phi1)*sin(phi2);
	
	double greatCircleDistance = REARTH.get(Length::M)*acos(cosTheta);

	Length distance(greatCircleDistance, Length::M);

	return distance;
}