//
// Created by Yash on 26/9/24.
//
#include "Data.hpp"

void checkNum(const char c) {
	if(c < '0' || c > '9') {
		throw std::runtime_error("Invalid number");
	}
}

void checkValue(const std::string value) {
	if(value[0] != '+' && value[0] != '-') {
		throw std::runtime_error("Invalid value format");
	}
	int numValue = std::atoi(value.substr(1).c_str());
	if(numValue < 0 ) {
		throw std::runtime_error("not a positive number");
	}
	else if (numValue > 1000) {
		throw std::runtime_error("too large a number.");
	}
}

void checkDate(const std::string line) {
	for(int i = 0; i <= 3; i++)
		checkNum(line[i]);
	if(line[4] != '-')
		throw std::runtime_error("Invalid date format");
	for(int i = 5; i <= 6; i++)
		checkNum(line[i]);
	if(line[7] != '-')
		throw std::runtime_error("Invalid date format");
	for(int i = 8; i <= 9; i++)
		checkNum(line[i]);
	checkValue(line.substr(13));
}


double my_atof(const char* str) {
	if (!str) return 0.0; // Null check

	double result = 0.0;
	double fraction = 0.0;
	int divisor = 1;
	bool is_fraction = false;
	bool has_exponent = false;
	int exponent_sign = 1;
	int exponent_value = 0;


	while (*str && isdigit(*str)) {
		result = result * 10 + (*str - '0');
		++str;
	}

	if (*str == '.') {
		++str;
		is_fraction = true;
		while (*str && isdigit(*str)) {
			fraction = fraction * 10 + (*str - '0');
			divisor *= 10;
			++str;
		}
	}

	if (is_fraction) {
		result += fraction / divisor;
	}

	if (*str == 'e' || *str == 'E') {
		has_exponent = true;
		++str;

		if (*str == '-') {
			exponent_sign = -1;
			++str;
		} else if (*str == '+') {
			++str;
		}
		while (*str && isdigit(*str)) {
			exponent_value = exponent_value * 10 + (*str - '0');
			++str;
		}
	}

	if (has_exponent) {
		double power = 1.0;
		for (int i = 0; i < exponent_value; ++i) {
			power *= 10;
		}
		if (exponent_sign < 0) {
			result /= power;
		} else {
			result *= power;
		}
	}

	return result;
}

float findNearestValue(const std::map<Data, float>& dateMap, const Data& target) {
	std::map<Data, float>::const_iterator it = dateMap.lower_bound(target);  // Finds the first element not less than target

	// If exact date found and value is non-zero
	if (it != dateMap.end() && it->first.getYear() == target.getYear() && it->first.getMonth() == target.getMonth() && it->first.getDay() == target.getDay() && it->second != 0.0f) {
		return it->second;
	}

	// Iterate backwards if we don't find a valid date or the value is zero
	while (it != dateMap.begin()) {
		--it;
		if (it->second != 0.0f) {
			return it->second;
		}
	}

	// If no valid date found, return -1.0f (or handle accordingly)
	return -1.0f;
}

void parseFile(const std::string filename, std::map<Data, float> dateMap) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		throw std::runtime_error("Error opening file");
	}
	std::string line;
	std::getline(file, line);
	if (line != "date | value") {
		throw std::runtime_error("Error: Invalid file format");
	}
	while(std::getline(file, line)) {
		checkDate(line);
		Data data(std::atoi(line.substr(0, 4).c_str()), std::atoi(line.substr(5, 2).c_str()), std::atoi(line.substr(8, 2).c_str()));
		float value = my_atof(line.substr(13).c_str());
		float nearestValue = findNearestValue(dateMap, data) * value;
		std::cout << data << "=> " << value << " = " << nearestValue << std::endl;
	}
}

int main(int argc, char **argv) {
	if(argc != 2) {
		std::cout << "Error: Invalid number of arguments" << std::endl;
		return 0;
	}
	std::map<Data, float> dateMap;
	std::ifstream data_file("data.csv");
	std::string line;

	while(std::getline(data_file, line)) {
		if (line != "date,exchange_rate") {
			Data data(std::atoi(line.substr(0, 4).c_str()), std::atoi(line.substr(5, 2).c_str()), std::atoi(line.substr(8, 2).c_str()));
			float value = my_atof(line.substr(11).c_str());
			dateMap[data] = value;
		}
	}
	try {
		parseFile(argv[1] , dateMap);
	}
	catch (std::exception &e) {
		std::cout << "Error: " << e.what() << std::endl;
		return 1;
	}
}