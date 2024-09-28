//
// Created by Yash on 26/9/24.
//

#ifndef CPP_09_DATA_HPP
#define CPP_09_DATA_HPP
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>

class Data {
	private:
		int _day;
		int _month;
		int _year;

	public:
		Data();
		Data(int day, int month, int year);
		~Data();
		Data(const Data &other);
		Data &operator=(const Data &other);
		void checkData(int day, int month, int year);
		bool operator<(const Data& other) const;
		friend std::ostream &operator<<(std::ostream &out, const Data &data);
		int getDay() const;
		int getMonth() const;
		int getYear() const;

		class BadInput : public std::exception {
			public:
				virtual const char *what() const throw() {
					return "bad input => ";
				}
		};

		class NegativeNum : public std::exception {
			public:
				virtual const char *what() const throw() {
					return "not a positive number";
				}
		};

		class InvalidDate : public std::exception {
			public:
				virtual const char *what() const throw() {
					return "Invalid date format";
				}
		};

		class InvalidMonth : public std::exception {
			public:
				virtual const char *what() const throw() {
					return "Invalid month range";
				}
		};
};

#endif //CPP_09_DATA_HPP
