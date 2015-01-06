//layer class

#ifndef LAYER
#define LAYER

#include <vector>
#include "Neuron.h"
#include "ProteinData.h"

class Layer {
public:
	Layer ();
	~Layer ();

	void initializeLayer (const int& size);
	
	void commandToLearn (const Layer& prevLayer);

	/*
	ONLY FOR INPUT LAYERS
	TO DO (maybe) - CREATE InputLayer : Layer class
	function sets outputs as data from protein data,
	and resets wage.
	*/
	void setOutput (const ProteinData* & proteinData);

	
	std::vector<const Neuron*> getNeurons ();
	int getLayerSize ();




private:
	std::vector <Neuron*> neurons;
	int layerSize;
	Layer* prevLayer;
};

#endif