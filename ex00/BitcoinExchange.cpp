#include "BitcoinExchange.hpp"

float BitcoinExchange::my_stof(const std::string& str) {
        float result = 0.0f;
        float fraction = 0.0f;
        float multiplier = 0.1f;
        bool negative = false;
        bool decimal_point = false;
        
        std::string::const_iterator it = str.begin();
        
        // Handle negative numbers
        if (*it == '-') {
            negative = true;
            ++it;
        }
        
        // Process each character
        for (; it != str.end(); ++it) {
            if (*it == '.') {
                if (decimal_point) {
                    // Multiple decimal points are invalid
                    throw std::runtime_error("Invalid float format");
                }
                decimal_point = true;
            } else if (*it >= '0' && *it <= '9') {
                if (decimal_point) {
                    fraction += (*it - '0') * multiplier;
                    multiplier *= 0.1f;
                } else {
                    result = result * 10.0f + (*it - '0');
                }
            } else {
                // Invalid character
                throw std::runtime_error("Invalid float format");
            }
        }
        
        result += fraction;
        if (negative) {
            result = -result;
        }
        
        return result;
    }

bool BitcoinExchange::isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

bool BitcoinExchange::isValidDate(const std::string& date) {
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

void BitcoinExchange::trim(std::string& str) {
    size_t first = str.find_first_not_of(" \t");
    size_t last = str.find_last_not_of(" \t");
    if (first != std::string::npos && last != std::string::npos)
        str = str.substr(first, last - first + 1);
    else
        str.clear();
}

std::string BitcoinExchange::formatFloat(float value, int precision) {
    std::ostringstream oss;
    oss << std::fixed;
    
    int factor = 1;
    for (int i = 0; i < precision; ++i) {
        factor *= 10;
    }
    value = floor(value * factor + 0.5) / factor;
    
    int wholePart = static_cast<int>(value);
    int fractionalPart = static_cast<int>((value - wholePart) * factor + 0.5);
    
    oss << wholePart << '.';
    oss << std::setw(precision) << std::setfill('0') << fractionalPart;
    
    return oss.str();
}

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) : dataMap(other.dataMap) {}

BitcoinExchange::~BitcoinExchange() {}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other) {
    if (this != &other) {
        dataMap = other.dataMap;
    }
    return *this;
}

void BitcoinExchange::parseLine(const std::string& line) {
    if(line != "date,exchange_rate") {
        std::string date = line.substr(0, line.find(','));
        std::string exchange_rate = line.substr(line.find(',') + 1);
        dataMap[date] = my_stof(exchange_rate);
    }
}

void BitcoinExchange::parseData(const std::string& filename) {
    std::ifstream data_file(filename.c_str());
    if (!data_file.is_open()) {
        throw std::runtime_error("Error: could not open file.");
    }
    
    std::string line;
    std::getline(data_file, line); // Skip header line
    while(std::getline(data_file, line)) {
        parseLine(line);
    }
}

std::string BitcoinExchange::findClosestDate(const std::string& targetDate) const {
    std::string closestDate;
    
    // Find the first date that is greater than the target date
    std::map<std::string, float>::const_iterator it = dataMap.lower_bound(targetDate);
    
    // If we found a date greater than or equal to the target, and it's not the first element
    if (it != dataMap.begin()) {
        // Move to the previous date (which is less than or equal to the target)
        --it;
    } else if (it == dataMap.end()) {
        // If no date is greater than or equal to the target, return the last date in the map
        it = --dataMap.end();
    }
    
    closestDate = it->first;
    std::cout << closestDate << std::endl;
    return closestDate;
}

void BitcoinExchange::processAndPrintDates(const std::string& date, float value) const {
    if (value < 0) {
        std::cout << "Error: not a positive number." << std::endl;
        return;
    } else if (value > 1000) {
        std::cout << "Error: too large a number." << std::endl;
        return;
    }

    std::map<std::string, float>::const_iterator dataIt = dataMap.find(date);
    if (dataIt == dataMap.end()) {
        std::string closestDate = findClosestDate(date);
        dataIt = dataMap.find(closestDate);
    }

    if (dataIt != dataMap.end()) {
        float exchangeRate = dataIt->second;
        float result = value * exchangeRate;
        std::cout << date << " => " << formatFloat(value, 2) << " = " << formatFloat(result, 2) << std::endl;
    } else {
        std::cout << "Error: date not found." << std::endl;
    }
}

void BitcoinExchange::processInput(const std::string& filename) {
    std::ifstream input_file(filename.c_str());
    if (!input_file.is_open()) {
        throw std::runtime_error("Error: could not open file.");
    }

    std::string line;
    std::getline(input_file, line); // Skip header line

    while(std::getline(input_file, line)) {
        if (line == "date | value") continue;

        size_t delimiterPos = line.find('|');
        if (delimiterPos == std::string::npos) {
            std::cout << "Error: bad input => " << line << std::endl;
            continue;
        }

        std::string date = line.substr(0, delimiterPos);
        std::string value = line.substr(delimiterPos + 1);

        trim(date);
        trim(value);

        if (date.empty() || value.empty()) {
            std::cout << "Error: bad input => " << line << std::endl;
            continue;
        }

        if (!isValidDate(date)) {
            std::cout << "Error: bad input => " << line << std::endl;
            continue;
        }

        try {
            float floatValue = my_stof(value);
            processAndPrintDates(date, floatValue);
        } catch (const std::exception& e) {
            std::cout << "Error: bad input => " << line << std::endl;
        }
    }
}