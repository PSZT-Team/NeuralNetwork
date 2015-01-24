#include "Layer.h"


Layer::Layer () {
}


Layer::~Layer () {
}

Layer::Layer (const unsigned int& size, const unsigned int& prevLayerSize) : layerSize(size) {
	initializeLayer (size, prevLayerSize);
}

void Layer::initializeLayer (const unsigned int & size, const unsigned int & prevLayerSize) {

	for (unsigned int i = 0; i < size; ++i)  {
		neurons.push_back (new Neuron (prevLayerSize));
	}
}

void Layer::commandToCalculate (const Layer& prevLayer) {

	for (auto &i : neurons)  {
		i->calculateOutput (prevLayer.getNeurons());
	}

}

void Layer::calculateOutput (const Layer& prevLayer) {
	for (auto &i : neurons) {
		i->calculateLastOutput (prevLayer.getNeurons ());
	}

}

void Layer::calculateErrors (const Layer& nextLayer) {
	
	for (unsigned int i = 0; i < neurons.size (); ++i ) {
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

void Layer::resetLayer () {

	for (auto &i : neurons) {
		i->randomWeight ();
	}

}

void Layer::setOutput (ProteinData* proteinData)  {
	neurons [0]->setOutput (proteinData->getData1());
	neurons [1]->setOutput (proteinData->getData2());
	neurons [2]->setOutput (proteinData->getData1());		
}


unsigned int Layer::getLayerSize () {
    return neurons.size ();
}

std::vector<Neuron*> Layer::getNeurons () const  {
	return neurons;
}

