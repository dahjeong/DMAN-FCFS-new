#include "Angle.h"

const string Angle::DEG = "DEG";
const string Angle::RAD = "RAD";

const double Angle::DEG2RAD = 3.141592 / 180.0;

const Angle Angle::ZERO(0.0, Angle::DEG);
const Angle Angle::NINETYinDEG(90.0, Angle::DEG);



Angle::Angle() { }

Angle::Angle(double angle, string unit) : angle(angle) {

	if (unit == Angle::DEG) {

		this->angle = angle*Angle::DEG2RAD;

	} else if (unit == Angle::RAD) {

		this->angle = angle;

	} else {

		cout << "Wrong input!" << endl;
		system("pause");;
	}
}

// accessor
double Angle::get(string unit) const {

	if (unit == Angle::DEG) {

		return angle/Angle::DEG2RAD;

	} else if (unit == Angle::RAD) {

		return angle;

	} else {

		cout << "Wrong input!" << endl;
		system("pause");;
		return -1000.0;
	}
}

// operator
Angle Angle::add(Angle newAngle) {

	return Angle(angle + newAngle.get(Angle::RAD), Angle::RAD);
}

Angle Angle::subtract(Angle newAngle) {

	return Angle(angle - newAngle.get(Angle::RAD), Angle::RAD);
}

Angle Angle::multiplyScalar(double c) {

	return Angle(angle * c, Angle::RAD);
}

// trigonometry
double Angle::sine() {
	
	return sin(angle);
}

double Angle::cosine() {

	return cos(angle);
}

double Angle::tangent() {

	return tan(angle);
}