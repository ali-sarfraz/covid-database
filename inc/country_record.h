/*************************************************************************
 * Author: Ali Sarfraz.													 *
 * Description: Container class for instantiating a country object.      *
 * Date: October 6th, 2020. 											 *
 ************************************************************************/

#ifndef INC_COVID_DATABASE_COUNTRY_RECORD_H_
#define INC_COVID_DATABASE_COUNTRY_RECORD_H_

#include <string>

namespace covid_database {
class country_record {
  public:
	country_record(std::string name,
	               std::string code,
	               std::string new_confirmed_cases,
	               std::string new_deaths,
	               std::string new_recovered_cases,
	               std::string total_confirmed_cases,
	               std::string total_deaths,
	               std::string total_recovered_cases) {
		name_                  = name;
		code_                  = code;
		new_confirmed_cases_   = stoi(new_confirmed_cases);
		new_deaths_            = stoi(new_deaths);
		new_recovered_cases_   = stoi(new_recovered_cases);
		total_confirmed_cases_ = stoi(total_confirmed_cases);
		total_deaths_          = stoi(total_deaths);
		total_recovered_cases_ = stoi(total_recovered_cases);
	}

	~country_record() {}

	std::string getName() { return name_; }
	std::string getCode() { return code_; }
	int getNewConfirmedCases() { return new_confirmed_cases_; }
	int getNewDeaths() { return new_deaths_; }
	int getNewReoveredCases() { return new_recovered_cases_; }
	int getTotalConfirmedCases() { return total_confirmed_cases_; }
	int getTotalDeaths() { return total_deaths_; }
	int getTotalRecoveredCases() { return total_recovered_cases_; }

  private:
	std::string name_;
	std::string code_;
	int new_confirmed_cases_;
	int new_deaths_;
	int new_recovered_cases_;
	int total_confirmed_cases_;
	int total_deaths_;
	int total_recovered_cases_;
};

}  // namespace covid_database

#endif
