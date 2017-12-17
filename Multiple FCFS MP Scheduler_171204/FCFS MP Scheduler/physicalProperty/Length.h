#pragma once
#include <string>
#include <iostream>

using namespace std;


class Length
{
public:
	static const string M;
	static const string FT;
	static const string IN;
	static const string NMI;

	static const double FT2M;
	static const double IN2M;
	static const double NMI2M;

	static const Length ZERO;
	static const Length NaN;

private:
	double l;	// always meter

public:
	Length();
	Length(double l, string unit);

	double get(string unit) const;

	Length add(Length newLength);
	Length subtract(Length newLength);
	Length multiplyScalar(double c);

};

