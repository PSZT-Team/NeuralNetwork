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

	Layer (const int& size, const int& prevLayerSize);

	void initializeLayer (const int& size, const int& prevLayerSize);
	
	void commandToCalculate (const Layer& prevLayer);

	/*
	ONLY FOR INPUT LAYERS
	TO DO (maybe) - CREATE InputLayer : Layer class
	function sets outputs as data from protein data,
	and resets wage.
	*/
	void setOutput ( ProteinData* proteinData);

	
	std::vector<Neuron*> getNeurons () const;
	int getLayerSize ();

private:
	std::vector <Neuron*> neurons;
	int layerSize;
};

#endif