//Neuron class designed for hidden and output layers

#ifndef NEURON
#define NEURON

#define DEFAULT_ALPHA 0.f  // 0 to 1, >0.5
#define DEFAULT_BETA 0.7f
#define DEFAULT_ETA 0.04f  // 0.01 to 0.65

#include <math.h>
#include <vector>
#include <iostream>
#include <ctime>


class Neuron {
public:
	// Network learning parameters.
	static float ALPHA;
	static float BETA;
	static float ETA;

	
	Neuron ();
	~Neuron ();

	Neuron (const int& prevLayerSize);
	
	/*
	sets sizes of weights vectors, set sum at 0 and so on
	*/
	void initializeNeuron (const int & prevLayerSize);

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
	Function calculates the output value from input data for linear layer.
	Input parameter are vector of neutrons from previous layer
	each neutron provides input value for this neutron, returning it's output value
	output value calculation : sum (weight * input) + free weight
	*/
	void calculateLastOutput (const std::vector<Neuron*> & prevLayerNeurons);
	
	/*
	function calculates output for hidden, sigmoidal layers, where activation function
	is hyperbolic tangent
	*/
	void calculateError (const std::vector<Neuron*> & prevNeuronLayers, const int & neuronNumber);

	/*
	fuction correct every weight.
	*/
	void correctWeight (const std::vector<Neuron*> & prevLayerNeurons);

	
	// GETTERS AND SETTERS
	
	/*
	gets weight which defines connection between neuron no. i from prev. layer with this neuron
	*/
	float getWeight (const int & i);

	/*
	gets calculated output
	necessary for next layer calculations
	*/
	float getOutput ();


	float getError ();

	// sets error only for output layer
	void setError (const float & result);

	// ONLY FOR INPUT LAYER
	void setOutput (const float & out);

	// Set learning parameters values.
	static void setParameters (const float alpha, const float beta, const float eta);


private:
	std::vector <float> weight;
	std::vector <float> prevWeight;
	
	float error;
	float output;
	float sum;
	
};


#endif 
