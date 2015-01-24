#include "Neuron.h"

float Neuron::ALPHA = DEFAULT_ALPHA;
float Neuron::BETA = DEFAULT_BETA;
float Neuron::ETA = DEFAULT_ETA;

Neuron::Neuron () {

}


Neuron::~Neuron () {
}

Neuron::Neuron (const int& prevLayerSize) {
	initializeNeuron (prevLayerSize);
	srand ((unsigned)time (NULL));
	randomWeight ();

}




void Neuron::initializeNeuron (const int & prevLayerSize) {
	for (int i = 0; i < prevLayerSize; ++i) {
		weight.push_back (0.0f);
		prevWeight.push_back (0.0f);
	}
}

void Neuron::randomWeight () {

	for (auto &i : weight) {
		i = (((rand () % 100000L) / 1700.f) - 9.8f)*0.0015f;
		//i = ((rand () % 1000L) / 10000.0);
		//i = rand () % 1L + 1;
		if (i == 0.f) i = 0.01492f;
		//i = 0.05;
	}
	for (auto &i : prevWeight) {
		i = 0.f;
	}

		

}

void Neuron::calculateOutput (const std::vector<Neuron*> & prevLayerNeurons) {
	output = 0;
	sum = 0;
	if (weight.size () != prevLayerNeurons.size ()) {
		//CRITICALL ERROR 
		std::cout << "CRITICAL ERROR" << std::endl;
		return;
	}

	for (unsigned int i = 0; i < weight.size (); ++i)  {
		sum += (weight [i] * prevLayerNeurons [i]->getOutput());
	}

	// Hyperbolic tangens
	output = tanh (BETA * sum);
}

void Neuron::calculateLastOutput (const std::vector<Neuron*> & prevLayerNeurons) {
	output = 0;
	sum = 0;
	if (weight.size () != prevLayerNeurons.size ()) {
		//CRITICALL ERROR 
		std::cout << "CRITICAL ERROR" << std::endl;
		return;
	}
	for (unsigned int i = 0; i < weight.size (); ++i) {
		sum += (weight [i] * prevLayerNeurons [i]->getOutput ());
	}
	output = sum;
}


void Neuron::calculateError (const std::vector<Neuron*> & nextNeuronLayer, const int& neuronNumber) {
	error = 0;

	for (unsigned int i = 0; i < nextNeuronLayer.size (); ++i) {

		// error += f'(x) * error[nextLayerAllNeurons]*weight[nextLayerAllNeurons][thisLayer]
		// error += (BETA * (1 - pow (tanh (BETA*sum), 2.0)) * (nextNeuronLayer[i]->getWeight (neuronNumber) * nextNeuronLayer[i]->getError()));
		error += nextNeuronLayer [i]->getWeight (neuronNumber) * nextNeuronLayer [i]->getError ();
	}
	error *= (BETA * (1 - (float) pow (tanh (BETA*sum), 2.0)));
	
}


void Neuron::correctWeight (const std::vector<Neuron*> & prevLayerNeurons) {

	if (weight.size () != prevLayerNeurons.size ()) {
		//CRITICALL ERROR 
		std::cout << "CRITICAL ERROR" << std::endl;
		return;
	}
	
	for (unsigned int i = 0; i < weight.size (); ++i) {

		float w = weight [i];
		weight [i] += ETA * error * prevLayerNeurons [i]->getOutput () + ALPHA*(weight [i] - prevWeight [i]);
		//weight [i] += ETA * error * prevLayerNeurons [i]->getOutput ();

		prevWeight [i] = w;
	}

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

void Neuron::setParameters (const float alpha, const float beta, const float eta) {
	ALPHA = alpha;
	BETA = beta;
	ETA = eta;
}

void Neuron::setError (const float & result) {
	error = (result - output);// *(BETA * (1 - pow (tanh (BETA*output), 2.0)));
}

float Neuron::getError () {
	return error;
}
