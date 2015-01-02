//@autgor Andrzej Siadkowski
//class responsible for loading and storage data from input file;
//dasdad
#ifndef DATACOLLECTOR
#define DATACOLLECTOR

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>   
#include <random>
#include "ProteinData.h"

class DataCollector {
public:
	DataCollector();
	DataCollector (const std::string&  filename);
	~DataCollector ();

	//loading data from file 
	bool loadData ();
	void getData ();

	void setFilename (const std::string filename);
	void shuffleData ();

	/*
	function returns vector with pointers to protein's data.
	param percentage is the percentage of data which will be used in lerning process
	returns first percentage% of data;
	*/
	std::vector <ProteinData*> getLearningData (const int & percentage);

	/*
	function returns vector with pointers to protein's data.
	param percentage is the percentage of data which will be used in testing and discovering process
	returns last percentage% of data, where every protein has 0 as a result of the reaction;
	*/
	std::vector <ProteinData*> getTestData (const int & percentage);
	
	/*
	function returns vector with pointers to protein's data.
	param percentage is the percentage of data which will be used in 
	checking the accuracy of testing process
	returns last percentage% of data;
	*/
	std::vector <ProteinData*> getCheckData (const int & percentage);

private:

	bool assignFile ();
	void getDataFromFile ();
	void closeFile ();
	
	void showData ();
	
	std::fstream dataFile;
	std::string file;
	
	std::vector <ProteinData*> data;
	int dataCounter;
};

#endif