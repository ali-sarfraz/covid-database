# COVID-19 Database
A simple graph generator for depicting COVID-19 stats.

## Features

- Graphical Depiction of Stats
- Sort Data in 6 Categories
- Ascending and Decending Ordering

## Run Locally

Clone the project

```bash
  git clone https://github.com/ali-sarfraz/covid-database
```

Compile the program

```bash
  cd inc && mv country_record.h ../src && mv utility.h ../src && cd ../src
  g++ main.cpp utility.cpp -o app
```

Run the program

```bash
  cd src
  ./app
```

## Demo

``` bash
Available data fields to sort by:
1: New Confirmed Cases
2: New Death Cases
3: New Recovered Cases  
4: Total Confirmed Cases  
5: Total Deaths          
6: Total Recovered         

Select field number to sort data by: 5      
Press 1 to sort data in Asending Order OR Press 2 to sort data in Descending Order: 2    
Sorting data in Descending Order.
```

![image](https://user-images.githubusercontent.com/98780551/152275771-e1f71725-38f4-4d82-8829-40a69d5f6953.png)  
