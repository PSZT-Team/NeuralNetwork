//@author Andrzej Siadkowski
//Neuron class designed for hidden and output layers

#ifndef NEURON
#define NEURON

#include <vector>

class Neuron {
public:
	Neuron ();
	~Neuron ();
	
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
	void calculateOutput (const std::vector<Neuron*> & prevNeuronLayers);
	
	/*
	sets sizes of weights vectors, set sum at 0 and so on
	*/
	void initializeNeuron (const int & prevLayerSize);

	/*
	predeclaration. don't know what arguments will be necessary yet.
	*/
	void correctWeight ();

	/*
	gets calculated output
	necessary for next layer calculations
	*/
	float getOutput ();



private:
	std::vector <float> weight;
	std::vector <float> prevWeight;
	
	float output;
	

};


#endif 
