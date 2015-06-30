//layer class

#ifndef LAYER
#define LAYER

#include <vector>
#include "Neuron.h"
#include "ProteinData.h"

class Layer
{
public:
    // Layer container (first - input, last - output and in between - hidden)
    typedef std::vector<Layer*> Layers;

    Layer();
    ~Layer();

    Layer(const unsigned int& size, const unsigned int& prevLayerSize);

    void initializeLayer(const unsigned int& size, const unsigned int& prevLayerSize);

    void commandToCalculate(const Layer& prevLayer);

    void calculateOutput(const Layer& prevLayer);

    void calculateErrors(const Layer& nextLayer);

    void correctWeights(const Layer& prevLayer);

    void outputLayerError(const int & output);

    void resetLayer();

    float getOutput();

    /*
     * Only for input layers.
     */
    void setOutput(ProteinData* proteinData);

    unsigned int getLayerSize();

    std::vector<Neuron*> getNeurons() const;

private:
    std::vector <Neuron*> neurons;
    int layerSize;
};

#endif