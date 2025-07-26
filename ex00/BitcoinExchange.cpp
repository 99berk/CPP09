#include "BitcoinExchange.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <climits>

BitcoinExchange::BitcoinExchange()
{
	loadDatabase("data.csv");
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other)
{
	*this = other;
}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other)
{
	if (this != &other) {
		exchangeRates = other.exchangeRates;
	}
	return *this;
}

BitcoinExchange::~BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const std::string& databaseFile)
{
	loadDatabase(databaseFile);
}

void BitcoinExchange::loadDatabase(const std::string& filename)
{
	std::ifstream file(filename.c_str());
	if (!file.is_open())
		throw std::runtime_error("Error: could not open database file.");

	std::string line;
	bool firstLine = true;
	int lineNumber = 0;
	
	while (std::getline(file, line))
	{
		lineNumber++;
		
		if (firstLine)
		{
			if (line != "date,exchange_rate")
				throw std::runtime_error("Error: invalid database header format.");
			firstLine = false;
			continue;
		}
		
		if (!isValidDatabaseFormat(line))
		{
			std::ostringstream oss;
			oss << "Error: invalid format at line " << lineNumber << ": " << line;
			throw std::runtime_error(oss.str());
		}
		
		size_t commaPos = line.find(',');
		if (commaPos == std::string::npos)
			continue;
		
		std::string date = trim(line.substr(0, commaPos));
		std::string rateStr = trim(line.substr(commaPos + 1));

		if (!isValidDate(date))
		{
			std::ostringstream oss;
			oss << "Error: invalid date format at line " << lineNumber << ": " << date;
			throw std::runtime_error(oss.str());
		}

		if (rateStr.empty()) {
            std::ostringstream oss;
            oss << "Error: missing rate value at line " << lineNumber << ": " << line;
            throw std::runtime_error(oss.str());
        }

		char* endPtr;
		float rate = std::strtof(rateStr.c_str(), &endPtr);
		if (*endPtr != '\0' || rate < 0)
		{
			std::ostringstream oss;
			oss << "Error: invalid rate value at line " << lineNumber << ": " << rateStr;
			throw std::runtime_error(oss.str());
		}
		
		exchangeRates[date] = rate;
	}
	file.close();
	
	if (exchangeRates.empty())
		throw std::runtime_error("Error: no valid data found in database file.");
}

void BitcoinExchange::processInputFile(const std::string& filename)
{
	std::ifstream file(filename.c_str());
	if (!file.is_open())
	{
		std::cerr << "Error: could not open file." << std::endl;
		return;
	}

	std::string line;
	bool firstLine = true;
	while (std::getline(file, line))
	{
		if (firstLine)
		{
			firstLine = false;
			continue;
		}
		
		size_t pipePos = line.find(" | ");
		if (pipePos == std::string::npos)
		{
			std::cerr << "Error: bad input => " << line << std::endl;
			continue;
		}
		
		std::string date = trim(line.substr(0, pipePos));
		std::string valueStr = trim(line.substr(pipePos + 3));
		
		if (!isValidDate(date))
		{
			std::cerr << "Error: bad input => " << date << std::endl;
			continue;
		}
		
		if (valueStr.empty())
		{
			std::cerr << "Error: bad input => " << line << std::endl;
			continue;
		}

		float value;
		if (!isValidValue(valueStr, value))
		{
			if (value < 0)
				std::cerr << "Error: not a positive number." << std::endl;
			else
				std::cerr << "Error: too large a number." << std::endl;
			continue;
		}
		
		float rate = getExchangeRate(date);
		float result = value * rate;
		std::cout << date << " => " << value << " = " << result << std::endl;
	}
	file.close();
}

float BitcoinExchange::getExchangeRate(const std::string& date) const
{
	std::map<std::string, float>::const_iterator it = exchangeRates.find(date);
	if (it != exchangeRates.end())
		return it->second;

	std::map<std::string, float>::const_iterator lower = exchangeRates.lower_bound(date);
	if (lower != exchangeRates.begin())
	{
		--lower;
		return lower->second;
	}
	return 0.0f;
}

bool BitcoinExchange::isValidDate(const std::string& date) const
{
	if (date.length() != 10 || date[4] != '-' || date[7] != '-')
		return false;
	
	for (size_t i = 0; i < date.length(); ++i)
	{
		if (i == 4 || i == 7) 
			continue;
		if (!std::isdigit(date[i])) 
			return false;
	}
	
	int year = std::atoi(date.substr(0, 4).c_str());
	int month = std::atoi(date.substr(5, 2).c_str());
	int day = std::atoi(date.substr(8, 2).c_str());
	
	if (year < 1 || month < 1 || month > 12 || day < 1 || day > 31)
		return false;
	return true;
}

bool BitcoinExchange::isValidValue(const std::string& valueStr, float& value) const
{
	char *endPtr;
	value = std::strtof(valueStr.c_str(), &endPtr);
	
	if (*endPtr != '\0')
		return false;
	if (value < 0)
		return false;
	if (value > 1000)
		return false;
	
	return true;
}

bool BitcoinExchange::isValidDatabaseFormat(const std::string& line) const
{
	size_t commaPos = line.find(',');

	if (commaPos == std::string::npos)
		return false;
	if (line.find(',', commaPos + 1) != std::string::npos)
		return false;
	if (commaPos == 0 || commaPos == line.length() - 1)
		return false;
	return true;
}

std::string BitcoinExchange::trim(const std::string& str) const
{
	size_t start = str.find_first_not_of(" \t\r\n");
	if (start == std::string::npos)
		return "";

	size_t end = str.find_last_not_of(" \t\r\n");
	return str.substr(start, end - start + 1);
}
