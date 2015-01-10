//@author Andrzej Siadkowski
//Neuron class designed for hidden and output layers

#ifndef NEURON
#define NEURON
#define BETA 0.7
#define ETA 0.04 // 0.01 to 0.6
#define ALPHA 0 // 0 to 1, >0.5

#include <math.h>
#include <vector>
#include <iostream>
#include <ctime>


class Neuron {
public:
	Neuron ();
	~Neuron ();
	Neuron (const int& prevLayerSize);
	
	/*
	function randomizes weight of each input information
	IMPORTANT AT THE BEGINING OF EVERY CROSS-VALIDATION PROCESS
	*/
	void randomWeight ();
	
	
	/*
	Function calculates the output value from input data.
	Input parameter are vector of neutrons from previous layer
	each neutron provides input value for this neutron, returning it's output value
	output value calculation : tgh (sum (weight * input) )
	*/
	void calculateOutput (const std::vector<Neuron*> & prevLayerNeurons);

	
	/*
	sets sizes of weights vectors, set sum at 0 and so on
	*/
	void initializeNeuron (const int & prevLayerSize);

	/*
	fuction correct every weight.
	*/
	void correctWeight (const std::vector<Neuron*> & prevLayerNeurons);

	void calculateError (const std::vector<Neuron*> & prevNeuronLayers, const int & neuronNumber);

	void setError (const float & result);


	/*
	gets calculated output
	necessary for next layer calculations
	*/
	float getOutput ();

	float getWeight (const int & i); 

	float getError ();

	// ONLY FOR INPUT LAYER
	void setOutput (const float & out);




private:
	std::vector <float> weight;
	std::vector <float> prevWeight;
	
	float error;
	float output;
	float sum;
	

};


#endif 
