
#ifndef BITCOINEXCHANGE_HPP

#include <ctime>
#include <cstdlib>
#include <cctype>
#include <cmath>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <climits>

class BitcoinExchange {
private:
    std::map<std::string, float> dataMap;

    static float my_stof(const std::string& str);
    static bool isLeapYear(int year);
    static bool isValidDate(const std::string& date);
    static void trim(std::string& str);
    static std::string formatFloat(float value, int precision);
    std::string findClosestDate(const std::string& targetDate) const;
    void parseLine(const std::string& line);
    void processAndPrintDates(const std::string& date, float value) const;

public:
    BitcoinExchange(); // Default constructor
    BitcoinExchange(const BitcoinExchange& other); // Copy constructor
    ~BitcoinExchange(); // Destructor

    BitcoinExchange& operator=(const BitcoinExchange& other); // Assignment operator

    void parseData(const std::string& filename);
    void processInput(const std::string& filename);
};

#endif