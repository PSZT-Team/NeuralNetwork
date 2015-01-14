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
	//void getData ();

	void setFilename (const std::string filename);
	void shuffleData ();

	/*
	function returns vector with pointers to protein's data, from which neutral network will be lerning.
	Params begin and length define part of data which 
	WILL
	NOT 
	BE 
	USED
	in lerning process.
	@param begin - first avoided data cell 
	@param length - length of avoided part of data
	*/
	std::vector <ProteinData*> getLearningData (const unsigned int & begin, const unsigned int & length );

	/*
	function returns vector with pointers to protein's data, which will be used in testing and discovering process
	@param begin - first data cell, which will be used in testing process
	@param length - lenght of data vector used in testing process
	*/
	std::vector <ProteinData*> getTestData (const unsigned int & begin, const unsigned int & length);
	
	/*
	function returns vector with pointers to protein's data, whitch will be used in checking the neutral network correctness 
	@param begin - first data cell, which will be used in checking process
	@param length - lenght of data vector used in checking process
		*/
	std::vector <ProteinData*> getCheckData (const unsigned int & begin, const unsigned int & length);

    // Getters

    unsigned int getDataCount ();   // Return number of all records.

private:

	bool assignFile ();
	void getDataFromFile ();
	void closeFile ();
	
	void showData ();
	
	std::fstream dataFile;
	std::string file;
	
	std::vector <ProteinData*> data;
};

#endif