#include "Layer.h"


Layer::Layer () {
}


Layer::~Layer () {
}

Layer::Layer (const int& size, const int& prevLayerSize) : layerSize(size) {
	initializeLayer (size, prevLayerSize);
}

void Layer::initializeLayer (const int & size, const int & prevLayerSize) {

	for (int i = 0; i < size; ++i)  {
		neurons.push_back (new Neuron (prevLayerSize));
	}
}

void Layer::commandToCalculate (const Layer& prevLayer) {

	for (auto &i : neurons)  {
		i->calculateOutput (prevLayer.getNeurons());
	}

}

void Layer::calculateErrors (const Layer& nextLayer) {
	
	for (int i = 0; i < neurons.size (); ++i ) {
		neurons[i]->calculateError (nextLayer.getNeurons (),i);
	}
}

void Layer::correctWeights (const Layer & prevLayer) {

	for (auto &i : neurons) {
		i->correctWeight (prevLayer.getNeurons ());

	}
}

void Layer::outputLayerError (const int & result) {

	neurons [0]->setError ((float) result);

}

void Layer::setOutput (ProteinData* proteinData)  {
	neurons [0]->setOutput (proteinData->getData1());
	neurons [1]->setOutput (proteinData->getData2());
	neurons [2]->setOutput (proteinData->getData1());		
}


int Layer::getLayerSize () {
	return layerSize;
}

std::vector<Neuron*> Layer::getNeurons () const  {
	return neurons;
}

