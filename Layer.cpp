#include "Layer.h"


Layer::Layer () {
}


Layer::~Layer () {
}


std::vector <const Neuron *> Layer::getNeurons ()  {
	return std::vector<const Neuron *> (neurons.begin (), neurons.end ());
}


int Layer::getLayerSize () {
	return neurons.size ();
}