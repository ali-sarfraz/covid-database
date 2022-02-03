/*************************************************************************
 * Author: Ali Sarfraz.											         *
 * Description: Supplementary class for providng utility funtions needed *
 * by the database.										                 *
 * Date: October 6th, 2020.										         *
 ************************************************************************/

#ifndef INC_COVID_DATABASE_UTILITY_H_
#define INC_COVID_DATABASE_UTILITY_H_

#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <vector>

#include "country_record.h"

namespace covid_database {
class utility {
  public:
	static void openAndReadFile(std::ifstream& file);
	static void parseDataIntoVector(std::ifstream& file,
	                                std::vector<country_record>& dataset);
	static bool isNameTokenComplete(std::string token);
	static std::string completeNameToken(std::string line);

	static void populateCountryVector(std::vector<std::string>& tokens,
	                                  std::vector<country_record>& dataset,
	                                  int& line_number);
	static bool validateNumber(std::string number);
	static int sortData(std::vector<country_record>& dataset);
	static void getSortParameters(int& field_number, int& sort_order);

	static void printGraph(std::vector<country_record>& dataset,
	                       int& field_number);
	static void accumulateData(std::vector<country_record>& dataset,
	                           std::vector<int>& data_to_print,
	                           int& field_number);
	static int calculateAndInsertBars(std::vector<country_record>& dataset,
	                                  std::vector<int>& data_to_print);
	static void insertFooter(int& field_number, int& bar_weightage);

	static bool compareNewConfirmed(country_record& first,
	                                country_record& second);
	static bool compareNewDeaths(country_record& first, country_record& second);
	static bool compareNewRecovered(country_record& first,
	                                country_record& second);
	static bool compareTotalConfirmed(country_record& first,
	                                  country_record& second);
	static bool compareTotalDeaths(country_record& first,
	                               country_record& second);
	static bool compareTotalRecovered(country_record& first,
	                                  country_record& second);
};

}  // namespace covid_database

#endif
