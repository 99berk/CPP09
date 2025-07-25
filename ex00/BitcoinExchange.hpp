#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <string>
#include <map>

class BitcoinExchange
{
	private:
		std::map<std::string, float> exchangeRates;
		void loadDatabase(const std::string& filename);
		float getExchangeRate(const std::string& date) const;
		bool isValidDate(const std::string& date) const;
		bool isValidValue(const std::string& valueStr, float& value) const;
		std::string trim(const std::string& str) const;
		bool isValidDatabaseFormat(const std::string& line) const;
		
	public:
		BitcoinExchange();
		BitcoinExchange(const BitcoinExchange& other);
		BitcoinExchange& operator=(const BitcoinExchange& other);
		~BitcoinExchange();
		BitcoinExchange(const std::string& databaseFile);
		
		void processInputFile(const std::string& filename);
};

#endif
