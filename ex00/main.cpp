//
// Created by Yash on 26/9/24.
//

#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cmath>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <climits>

float my_stof(std::string str) {
	float result = 0;
	int i = 0;
	int decimal = 0;
	int decimal_count = 0;

	while(str[i] != '\0') {
		if(str[i] == '.') {
			decimal = 1;
			i++;
			continue;
		}
		if(decimal) {
			result = result + (str[i] - '0') * pow(10, -decimal_count);
			decimal_count++;
		} else {
			result = result * 10 + (str[i] - '0');
		}
		i++;
	}
	return result;
}

void parseLine(std::string &line, std::map<std::string, float> &dataMap) {
	if(line != "date,exchange_rate") {
		std::string date = line.substr(0, line.find(','));
		std::string exchange_rate = line.substr(line.find(',') + 1);
		dataMap[date] = my_stof(exchange_rate);
	}
}

void parseData(std::ifstream &data_file, std::map<std::string, float> &dataMap) {
	std::string line;

	std::getline(data_file, line);
	while(std::getline(data_file, line)) {
		parseLine(line, dataMap);
	}
}

bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

bool isValidDate(const std::string& date) {
    if (date.length() != 10) return false;
    if (date[4] != '-' || date[7] != '-') return false;

    for (int i = 0; i < 10; ++i) {
        if (i == 4 || i == 7) continue;
        if (!isdigit(date[i])) return false;
    }

    int year = atoi(date.substr(0, 4).c_str());
    int month = atoi(date.substr(5, 2).c_str());
    int day = atoi(date.substr(8, 2).c_str());

    if (year < 1900 || month < 1 || month > 12 || day < 1) return false;

    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (isLeapYear(year)) daysInMonth[1] = 29;

    return day <= daysInMonth[month - 1];
}

void trim(std::string& str) {
    size_t first = str.find_first_not_of(" \t");
    size_t last = str.find_last_not_of(" \t");
    if (first != std::string::npos && last != std::string::npos)
        str = str.substr(first, last - first + 1);
    else
        str.clear();
}

void checkInput(std::string& line, std::map<std::string, float>& inputMap) {
    if (line != "date | value") {
        size_t delimiterPos = line.find('|');
        if (delimiterPos == std::string::npos) {
            throw std::invalid_argument("Error: Invalid input format");
        }

        std::string date = line.substr(0, delimiterPos);
        std::string value = line.substr(delimiterPos + 1);
        
        trim(date);
        trim(value);

        if (date.empty() || value.empty()) {
            throw std::invalid_argument("Error: Invalid input file");
        }

        if (!isValidDate(date)) {
            throw std::invalid_argument("Error: Invalid date format or date. Use YYYY-MM-DD.");
        }

        float floatValue = my_stof(value);
        if (floatValue < 0) {
            throw std::invalid_argument("Error: not a positive number.");
        } else if (floatValue > 1000) {
            throw std::invalid_argument("Error: too large a number.");
        }
		inputMap.insert(std::pair<std::string, float>(date, floatValue));
    }
}

std::string findClosestDate(const std::string& targetDate, const std::map<std::string, float>& dataMap) {
    std::string closestDate;
    int smallestDiff = INT_MAX;

    for (std::map<std::string, float>::const_iterator it = dataMap.begin(); it != dataMap.end(); ++it) {
        int yearDiff = abs(atoi(targetDate.substr(0, 4).c_str()) - atoi(it->first.substr(0, 4).c_str()));
        int monthDiff = abs(atoi(targetDate.substr(5, 2).c_str()) - atoi(it->first.substr(5, 2).c_str()));
        int dayDiff = abs(atoi(targetDate.substr(8, 2).c_str()) - atoi(it->first.substr(8, 2).c_str()));
        
        int totalDiff = yearDiff * 365 + monthDiff * 30 + dayDiff;
        
        if (totalDiff < smallestDiff) {
            smallestDiff = totalDiff;
            closestDate = it->first;
        }
    }

    return closestDate;
}

void processAndPrintDates(const std::map<std::string, float>& inputMap, const std::map<std::string, float>& dataMap) {
    for (std::map<std::string, float>::const_iterator it = inputMap.begin(); it != inputMap.end(); ++it) {
        std::string date = it->first;
        float value = it->second;

        std::cout << date << " | " << value << " => ";

        std::map<std::string, float>::const_iterator dataIt = dataMap.find(date);
        if (dataIt == dataMap.end()) {
            std::string closestDate = findClosestDate(date, dataMap);
            dataIt = dataMap.find(closestDate);
            if (dataIt != dataMap.end()) {
                std::cout << closestDate << " = ";
            }
        }

        if (dataIt != dataMap.end()) {
            float exchangeRate = dataIt->second;
            float result = value * exchangeRate;
            std::cout << result << std::endl;
        } else {
            std::cout << "Error: date not found." << std::endl;
        }
    }
}

int main(int argc, char **argv) {
    if(argc != 2) {
        std::cout << "Error: Invalid number of arguments" << std::endl;
        return 1;
    }
    std::map<std::string, float> dataMap;
    std::ifstream data_file("data.csv");
    
    parseData(data_file, dataMap);

    std::map<std::string, float> inputMap;
    std::ifstream input_file(argv[1]);

    std::string line;
    std::getline(input_file, line); // Skip header line

    while(std::getline(input_file, line)) {
        try {
            checkInput(line, inputMap);
        } catch(const std::exception &e) {
            std::cout << line << " => " << e.what() << std::endl;
        }
    }

    processAndPrintDates(inputMap, dataMap);

    return 0;
}
