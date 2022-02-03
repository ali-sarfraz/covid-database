/*************************************************************************
 * Author: Ali Sarfraz.											         *
 * Description: Main function to execute the COVID-19 sorting database.  *
 * Date: October 6th, 2020.										         *
 ************************************************************************/

#include "country_record.h"
#include "utility.h"

using namespace std;

int main() {
	ifstream file;
	vector<covid_database::country_record> dataset;

	// Stage 1: Open and Read File.
	covid_database::utility::openAndReadFile(file);

	// Stage 2: Parse and Store.
	covid_database::utility::parseDataIntoVector(file, dataset);

	// Stage 3: Sorting Data.
	auto field_number = covid_database::utility::sortData(dataset);

	// Stage 4: Printing Graph.
	covid_database::utility::printGraph(dataset, field_number);

	file.close();
	return 0;
}
