//
// Created by Yash on 26/9/24.
//

#include "Data.hpp"

Data::Data() : _day(0), _month(0), _year(0) {}

void Data::checkData(int day, int month, int year) {
	static const int monthDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	if(year < 2009 || month < 1 || month > 12) {
		throw InvalidDate();
	}
	if(day > monthDays[month - 1] || day < 1) {
		throw InvalidMonth();
	}
}

Data::Data(int year, int month, int day){
	try{
		checkData(day, month, year);
	}catch (const std::exception &e) {
		std::cout << "Error: " << e.what();
		if (dynamic_cast<const BadInput*>(&e) != nullptr) {
			std::cout << " " << _year << "-" << _month << "-" << _day;
		}
		std::cout << std::endl;
		_day = 0;
		_month = 0;
		_year = 0;
	}
}

Data::~Data() {}

Data::Data(const Data &other) : _day(other._day), _month(other._month), _year(other._year) {}

Data &Data::operator=(const Data &other) {
	_day = other._day;
	_month = other._month;
	_year = other._year;
	return *this;
}

bool Data::operator<(const Data& other) const {
	if (_year != other._year) {
		return _year < other._year;
	} else if (_month != other._month) {
		return _month < other._month;
	} else {
		return _day < other._day;
	}
}

int Data::getDay() const {
	return _day;
}

int Data::getMonth() const {
	return _month;
}

int Data::getYear() const {
	return _year;
}

std::ostream &operator<<(std::ostream &out, const Data &data) {
	out << data._year << "-" << data._month << "-" << data._day;
	return out;
}