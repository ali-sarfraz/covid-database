/*************************************************************************
 * Author: Ali Sarfraz.											         *
 * Description: Implementation of methods provided in the utility class. *
 * Date: October 6th, 2020.										         *
 ************************************************************************/

#include "utility.h"

using namespace std;

// Define constexprs for use instead of arbitrary numbers.
static constexpr char delim                      = ',';
static constexpr size_t expected_tokens_per_line = 11;
static constexpr size_t file_error_code          = 69;
static constexpr size_t console_char_limit       = 70;
static constexpr size_t input_buffer_clear_size  = 6969;

// Used for accessing members in each line of CSV file.
static constexpr size_t index_of_name            = 0;
static constexpr size_t index_of_code            = 1;
static constexpr size_t index_of_new_confirmed   = 3;
static constexpr size_t index_of_new_deaths      = 4;
static constexpr size_t index_of_new_recovered   = 5;
static constexpr size_t index_of_total_confirmed = 8;
static constexpr size_t index_of_total_deaths    = 9;
static constexpr size_t index_of_total_recovered = 10;

// Used for identifying user selction for what field to sort by.
static constexpr size_t new_confirmed_sort   = 1;
static constexpr size_t new_deaths_sort      = 2;
static constexpr size_t new_recovered_sort   = 3;
static constexpr size_t total_confirmed_sort = 4;
static constexpr size_t total_deaths_sort    = 5;

// Used for identifying user selection for what order to sort in.
static constexpr size_t ascending = 1;

namespace covid_database {

/**
 * Func Name: openAndReadFile.
 * Description: Opens a user in the project directory based on user input for
 * the name.
 * Parameters: Takes a reference to a file object.
 * Return Type: N/A.
 */
void utility::openAndReadFile(ifstream& file) {
	string file_name;
	cout << "Welcome to the COVID-19 Data Interpreter!" << endl;
	cout << "Enter the data filename: ";
	cin >> file_name;

	file.open(file_name);

	while (!file.is_open()) {
		cerr << "Error: Filename '" << file_name
		     << "' does not exist in current directory!" << endl;
		cout << "Enter the data filename: ";

		// Clear input buffer in case there are invaid chars left behind.
		cin.clear();
		cin.ignore(input_buffer_clear_size, '\n');

		cin >> file_name;
		file.open(file_name);
	}

	cout << "File opened successfully!\n" << endl;

	// Check to see if the file is empty.
	if (file.peek() == ifstream::traits_type::eof()) {
		cerr << "Error: file is empty!" << endl;
		file.close();
		exit(file_error_code);
	}
}

/**
 * Func Name: parseDataIntoVector.
 * Description: Takes data from CSV file and places it into a vector.
 * Parameters: Takes a reference to a file object and a reference to a vector to
 * store the data in.
 * Return Type: N/A.
 */
void utility::parseDataIntoVector(ifstream& file,
                                  vector<country_record>& dataset) {
	// Dirty bool for bypassing the first line of the CSV file.
	bool first_line_not_parsed = true;
	// Line tracker to pinpoint which country we are on.
	int line_count = 2;
	string line;

	while (getline(file, line)) {
		string current_token;
		vector<string> tokens_in_line;
		istringstream s_stream(line);
		bool name_token_parsed = false;

		// Error checking to ensure file formatting is correct.
		auto format_correct = line[0] == '\"';
		if (!format_correct) {
			cerr << "Error: file format incorrect!" << endl;
			file.close();
			exit(file_error_code);
		}

		while (getline(s_stream, current_token, delim)) {
			// Look for instances of " " to isolate name tokens.
			if (current_token.find("\"") != string::npos) {
				if (isNameTokenComplete(current_token)) {
					tokens_in_line.push_back(current_token);
				}

				else {
					// Continue if name has already been parsed in order to
					// avoid parsing token twice, once for each instance of " in
					// the name string.
					if (name_token_parsed) { continue; }

					auto completed_name = completeNameToken(s_stream.str());
					tokens_in_line.push_back(completed_name);
					name_token_parsed = true;
				}
			} else {
				tokens_in_line.push_back(current_token);
			}
		}

		// Error checking to see if enough tokens in each line.
		if (tokens_in_line.size() != expected_tokens_per_line) {
			cerr << "Error: Not enough data provided on line " << line_count
			     << endl;
			file.close();
			exit(file_error_code);
		}

		// Don't add first line obtained from file, as it's not useful.
		if (first_line_not_parsed) {
			first_line_not_parsed = false;
			continue;
		}

		populateCountryVector(tokens_in_line, dataset, line_count);
		line_count++;
	}
}

/**
 * Func Name: isNameTokenComplete.
 * Description: Checks if a tokenized string has two " characters.
 * Parameters: Takes a tokenized string.
 * Return Type: True if string has two " chars, False otherwise.
 */
bool utility::isNameTokenComplete(string token) {
	int quote_count = 0;
	for (auto i = 0; i < token.size(); i++) {
		if (token[i] == '\"') { quote_count++; }
	}
	if (quote_count == 2) { return true; }

	return false;
}

/**
 * Func Name: completeNameToken.
 * Description: Completes name token for country names containing a , char.
 * Parameters: Takes a string containing the entire line of the file.
 * Return Type: Completed string name token including the , character.
 */
string utility::completeNameToken(string line) {
	auto second_quote_position = line.find('\"', 1);
	return line.substr(0, second_quote_position + 1);
}

/**
 * Func Name: populateCountryVector.
 * Description: Creates an instance of a country_record, and pushes it into a
 * vector.
 * Parameters: Takes a reference to a vector of strings containing the
 * tokenized line, and the destination vector of country_record to write this
 * data into.
 * Return Type: N/A.
 */
void utility::populateCountryVector(vector<string>& tokens,
                                    vector<country_record>& dataset,
                                    int& line_number) {
	// Error checking for numeric values.
	if (!validateNumber(tokens.at(index_of_new_confirmed)) ||
	    !validateNumber(tokens.at(index_of_new_deaths)) ||
	    !validateNumber(tokens.at(index_of_new_recovered)) ||
	    !validateNumber(tokens.at(index_of_total_confirmed)) ||
	    !validateNumber(tokens.at(index_of_total_deaths)) ||
	    !validateNumber(tokens.at(index_of_total_recovered))) {
		cerr << "Error: Invalid data detected on line " << line_number << endl;
		exit(file_error_code);
	}

	country_record data{tokens.at(index_of_name),
	                    tokens.at(index_of_code),
	                    tokens.at(index_of_new_confirmed),
	                    tokens.at(index_of_new_deaths),
	                    tokens.at(index_of_new_recovered),
	                    tokens.at(index_of_total_confirmed),
	                    tokens.at(index_of_total_deaths),
	                    tokens.at(index_of_total_recovered)};

	dataset.push_back(data);
}

/**
 * Func Name: validateNumber.
 * Description: Validates that numbers dont have characters.
 * Parameters: Takes a string containing the number.
 * Return Type: True if valid, false otherwise.
 */
bool utility::validateNumber(string number) {
	if (regex_search(number, regex("[^0-9]"))) { return false; }
	return true;
}

/**
 * Func Name: sortData.
 * Description: Sorts vector of data based on user input selections.
 * Parameters: Takes a reference to the data vector.
 * Return Type: Sorting field number selected by user through menu.
 */
int utility::sortData(vector<country_record>& dataset) {
	int field_number;
	int sort_order;

	getSortParameters(field_number, sort_order);

	switch (field_number) {
		case new_confirmed_sort:
			std::sort(dataset.begin(), dataset.end(), compareNewConfirmed);
			break;
		case new_deaths_sort:
			std::sort(dataset.begin(), dataset.end(), compareNewDeaths);
			break;
		case new_recovered_sort:
			std::sort(dataset.begin(), dataset.end(), compareNewRecovered);
			break;
		case total_confirmed_sort:
			std::sort(dataset.begin(), dataset.end(), compareTotalConfirmed);
			break;
		case total_deaths_sort:
			std::sort(dataset.begin(), dataset.end(), compareTotalDeaths);
			break;
		default:
			std::sort(dataset.begin(), dataset.end(), compareTotalRecovered);
	}

	// Reverse the sorted vector if output needs to be descending.
	if (sort_order != ascending) { reverse(dataset.begin(), dataset.end()); }

	return field_number;
}

/**
 * Func Name: getSortParameters.
 * Description: Accepts data sorting parameters from user through menu.
 * Parameters: Takes a reference to two ints for storing user selections.
 * Return Type: N/A.
 */
void utility::getSortParameters(int& field_number, int& sort_order) {
	cout << "Available data fields to sort by: " << endl;
	cout << "1: New Confirmed Cases" << endl;
	cout << "2: New Death Cases " << endl;
	cout << "3: New Recovered Cases" << endl;
	cout << "4: Total Confirmed Cases " << endl;
	cout << "5: Total Deaths " << endl;
	cout << "6: Total Recovered \n" << endl;

	cout << "Select field number to sort data by: ";
	cin >> field_number;

	while (field_number < 1 || field_number > 6) {
		cerr << "Error: Invalid selection! " << endl;
		cout << "Select field number to sort data by: ";

		// Clear input buffer in case user inputs a char.
		cin.clear();
		cin.ignore(input_buffer_clear_size, '\n');
		cin >> field_number;
	}

	cout << "Press 1 to sort data in Asending Order OR Press 2 to sort "
	        "data in Descending Order: ";
	cin >> sort_order;

	while (sort_order != 1 && sort_order != 2) {
		cerr << "Invalid selection! " << endl;
		cout << "Press 1 to sort data in Asending Order OR Press 2 to "
		        "sort data in Descending Order: ";

		// Clear input buffer, in case user inputs a char.
		cin.clear();
		cin.ignore(input_buffer_clear_size, '\n');
		cin >> sort_order;
	}

	if (sort_order == ascending) {
		cout << "Sorting data in Ascending Order.\n" << endl;
	} else {
		cout << "Sorting data in Descending Order.\n" << endl;
	}
}

/**
 * Func Name: printGraph.
 * Description: Prints a horizontal bar chart for top 10 values based on what
 * field the user selected.
 * Parameters: Takes a reference to the country vector and field number.
 * Return Type: N/A.
 */
void utility::printGraph(vector<country_record>& dataset, int& field_number) {
	vector<int> data_to_print;
	accumulateData(dataset, data_to_print, field_number);

	auto bar_weightage = calculateAndInsertBars(dataset, data_to_print);
	insertFooter(field_number, bar_weightage);
}

/**
 * Func Name: accumulateData.
 * Description: Accumulates top 10 values from country vector into data_to_print
 * based on what field the user selected.
 * Parameters: Takes a reference to country data, printing data, and field
 * number.
 * Return Type: N/A.
 */
void utility::accumulateData(vector<country_record>& dataset,
                             vector<int>& data_to_print,
                             int& field_number) {
	switch (field_number) {
		case new_confirmed_sort:
			for (auto i = 0; i < 10; i++) {
				data_to_print.push_back(dataset.at(i).getNewConfirmedCases());
			}
			break;
		case new_deaths_sort:
			for (auto i = 0; i < 10; i++) {
				data_to_print.push_back(dataset.at(i).getNewDeaths());
			}
			break;
		case new_recovered_sort:
			for (auto i = 0; i < 10; i++) {
				data_to_print.push_back(dataset.at(i).getNewReoveredCases());
			}
		case total_confirmed_sort:
			for (auto i = 0; i < 10; i++) {
				data_to_print.push_back(dataset.at(i).getTotalConfirmedCases());
			}
			break;
		case total_deaths_sort:
			for (auto i = 0; i < 10; i++) {
				data_to_print.push_back(dataset.at(i).getTotalDeaths());
			}
			break;
		default:
			for (auto i = 0; i < 10; i++) {
				data_to_print.push_back(dataset.at(i).getTotalRecoveredCases());
			}
	}
}

/**
 * Func Name: calculateAndInsertBars.
 * Description: Calculates the weightage of each # and and inserts them
 * accordingly.
 * Parameters: Takes a reference to country data and printing data.
 * Return Type: The weightage of each # as an int.
 */
int utility::calculateAndInsertBars(vector<country_record>& dataset,
                                    vector<int>& data_to_print) {
	int max_value     = 0;
	int bars_to_print = 0;
	int bar_weightage = 0;

	// Limit the max number of #s to 70.
	int max_bar_len = console_char_limit;

	for (auto i = 0; i < 10; i++) {
		if (data_to_print.at(i) > max_value) {
			max_value = data_to_print.at(i);
		}
	}

	// Ensure that max_value is longest bar when it's under 70.
	if (max_value < max_bar_len) { max_bar_len = max_value; }

	// Ensure we don't divide by 0.
	if (max_bar_len != 0) { bar_weightage = max_value / max_bar_len; }

	for (auto i = 0; i < 10; i++) {
		string bars;
		auto name = dataset.at(i).getCode();

		if (bar_weightage != 0) {
			bars_to_print = data_to_print.at(i) / bar_weightage;
		}

		// Do string manipulations for removing the " " in country code.
		cout << name.substr(1, name.size() - 2) + " | " +
		           bars.insert(0, bars_to_print, '#')
		     << endl;
		cout << "   |" << endl;
	}

	return bar_weightage;
}

/**
 * Func Name: insertFooter.
 * Description: Inserts a footer to the graph, including a summary.
 * Parameters: Takes a reference to field number and bar weightage.
 * Return Type: N/A.
 */
void utility::insertFooter(int& field_number, int& bar_weightage) {
	string footer;
	footer.insert(0, console_char_limit, '-');
	cout << footer << endl;

	switch (field_number) {
		case new_confirmed_sort:
			cout << "New Confirmed Cases; Each # is approx. " << bar_weightage
			     << " cases." << endl;
			break;
		case new_deaths_sort:
			cout << "New Death Cases; Each # is approx. " << bar_weightage
			     << " cases." << endl;
			break;
		case new_recovered_sort:
			cout << "New Recovered Cases; Each # is approx. " << bar_weightage
			     << " cases." << endl;
			break;
		case total_confirmed_sort:
			cout << "Total Confirmed Cases; Each # is approx. " << bar_weightage
			     << " cases." << endl;
			break;
		case total_deaths_sort:
			cout << "Total Deaths; Each # is approx. " << bar_weightage
			     << " cases." << endl;
			break;
		default:
			cout << "Total Recovered; Each # is approx. " << bar_weightage
			     << " cases." << endl;
	}
}

/**
 * Func Name: compareNewConfirmed.
 * Description: Compare two countries based on new confirmed cases.
 * Parameters: Two country_record objects.
 * Return Type: True if first country has a lower value.
 */
bool utility::compareNewConfirmed(country_record& first,
                                  country_record& second) {
	return (first.getNewConfirmedCases() < second.getNewConfirmedCases());
}

/**
 * Func Name: compareNewDeaths.
 * Description: Compare two countries based on new death cases.
 * Parameters: Two country_record objects.
 * Return Type: True if first country has a lower value.
 */
bool utility::compareNewDeaths(country_record& first, country_record& second) {
	return (first.getNewDeaths() < second.getNewDeaths());
}

/**
 * Func Name: compareNewRecovered.
 * Description: Compare two countries based on new recovered cases.
 * Parameters: Two country_record objects.
 * Return Type: True if first country has a lower value.
 */
bool utility::compareNewRecovered(country_record& first,
                                  country_record& second) {
	return (first.getNewReoveredCases() < second.getNewReoveredCases());
}

/**
 * Func Name: compareTotalConfirmed.
 * Description: Compare two countries based on total confirmed cases.
 * Parameters: Two country_record objects.
 * Return Type: True if first country has a lower value.
 */
bool utility::compareTotalConfirmed(country_record& first,
                                    country_record& second) {
	return (first.getTotalConfirmedCases() < second.getTotalConfirmedCases());
}

/**
 * Func Name: compareTotalDeaths.
 * Description: Compare two countries based on total death cases.
 * Parameters: Two country_record objects.
 * Return Type: True if first country has a lower value.
 */
bool utility::compareTotalDeaths(country_record& first,
                                 country_record& second) {
	return (first.getTotalDeaths() < second.getTotalDeaths());
}

/**
 * Func Name: compareTotalRecovered.
 * Description: Compare two countries based on total recovered cases.
 * Parameters: Two country_record objects.
 * Return Type: True if first country has a lower value.
 */
bool utility::compareTotalRecovered(country_record& first,
                                    country_record& second) {
	return (first.getTotalRecoveredCases() < second.getTotalRecoveredCases());
}

}  // namespace covid_database
