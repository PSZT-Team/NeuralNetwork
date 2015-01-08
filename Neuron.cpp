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

void Neuron::calculateError (const std::vector<Neuron*> & nextNeuronLayer, const int& neuronNumber) {
	error = 0;

	for (int i = 0; i < nextNeuronLayer.size (); ++i) {

		// error += f'(x) * error[nextLayerAllNeurons]*weight[nextLayerAllNeurons][thisLayer]
		error += (BETA * (1 - tanh (BETA*output) * tanh (BETA*output))) * (nextNeuronLayer[i]->getWeight (neuronNumber) * nextNeuronLayer[i]->getError());
	}
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


float Neuron::getWeight (const int & i) {
	return weight [i];
}


float Neuron::getOutput () {
	return output;
}

void Neuron::setOutput (const float & out) {
	output = out;
}

void Neuron::setError (const float & result) {

	error = result - output;
}

float Neuron::getError () {
	return error;
}
