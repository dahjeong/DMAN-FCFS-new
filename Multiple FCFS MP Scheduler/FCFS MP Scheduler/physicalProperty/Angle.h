#pragma once
#include <string>
#include <iostream>

using namespace std;

class Angle
{
public:
	static const string DEG;
	static const string RAD;

	static const double DEG2RAD;

	static const Angle ZERO;
	static const Angle NINETYinDEG;

private:
	double angle;	// always radian

public:
	
	// constructor
	Angle();
	Angle(double angle, string unit);

	// accessor
	double get(string unit) const;

	// operator
	Angle add(Angle newAngle);
	Angle subtract(Angle newAngle);
	Angle multiplyScalar(double c);

	// trygonometry
	double sine();
	double cosine();
	double tangent();
};

