#include "Neuron.h"


Neuron::Neuron () {

}


Neuron::~Neuron () {
}

Neuron::Neuron (const int& prevLayerSize) {
	initializeNeuron (prevLayerSize);
	randomWeight ();
}

void Neuron::randomWeight () {
	// TO DO!

}

void Neuron::calculateOutput (const std::vector<Neuron*> & prevNeuronLayers) {
	output = 0;
	if (weight.size () != prevNeuronLayers.size ()) {
		//CRITICALL ERROR 
		std::cout << "CRITICAL ERROR" << std::endl;
		return;
	}

	for (int i = 0; i < weight.size (); ++i)  {
		output += (weight [i] * prevNeuronLayers [i]->getOutput());
	}

	// Hyperbolic tangens
	output = tanh (BETA * output);


}

void Neuron::initializeNeuron (const int & prevLayerSize) {
	for (int i = 0; i < prevLayerSize; ++i)  {
		weight.push_back (0.0f);
		prevWeight.push_back (0.0f);
	}


}

void Neuron::correctWeight () {
	// TO DO! TO DO! TO DO!
}

float Neuron::getOutput () {
	return output;
}

void Neuron::setOutput (const float & out) {
	output = out;
}

