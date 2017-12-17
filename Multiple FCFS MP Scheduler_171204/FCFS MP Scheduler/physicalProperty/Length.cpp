#include "Length.h"

const string Length::M = "M";
const string Length::FT = "FT";
const string Length::IN = "IN";
const string Length::NMI = "NMI";

const double Length::FT2M = 0.3048;
const double Length::IN2M = 0.3048/12.0;
const double Length::NMI2M = 1852.0;

const Length Length::ZERO(0.0, Length::M);
const Length Length::NaN(-1.0, Length::M);


Length::Length() { }

Length::Length(double l, string unit) : l(l) {

	if (unit == Length::M) {

		this->l = l;

	} else if (unit == Length::FT) {

		this->l = l*Length::FT2M;

	} else if (unit == Length::IN) {

		this->l = l*Length::IN2M;

	} else if (unit == Length::NMI) {

		this->l = l*Length::NMI2M;
	
	} else {

		cout << "Wrong input!" << endl;
		system("pause");;
	}
}

double Length::get(string unit) const {

	if (unit == Length::M) {

		return l;

	} else if (unit == Length::FT) {

		return l/Length::FT2M;

	} else if (unit == Length::IN) {

		return l/Length::IN2M;

	} else if (unit == Length::NMI) {

		return l/Length::NMI2M;

	} else {

		cout << "Wrong input!" << endl;
		system("pause");;
		return -1.0;
	}
}

Length Length::add(Length newLength) {

	return Length(l + newLength.get(Length::M), Length::M);
}

Length Length::subtract(Length newLength) {

	return Length(l - newLength.get(Length::M), Length::M);
}

Length Length::multiplyScalar(double c) {

	return Length(l*c, Length::M);
}