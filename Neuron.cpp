#include "Neuron.h"

float Neuron::ALPHA = DEFAULT_ALPHA;
float Neuron::BETA = DEFAULT_BETA;
float Neuron::ETA = DEFAULT_ETA;

Neuron::Neuron()
{

}

Neuron::~Neuron()
{}

Neuron::Neuron(const int& prevLayerSize)
{
    initializeNeuron(prevLayerSize);
    srand((unsigned)time(NULL));
    randomWeight();
}


void Neuron::initializeNeuron(const int & prevLayerSize)
{
    for (int i = 0; i <= prevLayerSize; ++i)
    {
        weight.push_back(0.0f);
        prevWeight.push_back(0.0f);
    }
}

void Neuron::randomWeight()
{
    for (int i = 0; i < weight.size(); ++i)
    {
        weight[i] = (((rand() % 100000L) / 1700.f) - 9.8f)*0.0015f;
        if (weight[i] == 0.f)
            weight[i] = 0.01492f;
        prevWeight[i] = weight[i];
    }
}

void Neuron::calculateOutput(const std::vector<Neuron*> & prevLayerNeurons)
{
    output = 0;
    sum = 0;
    if ((weight.size() - 1) != prevLayerNeurons.size())
    {
        //CRITICAL ERROR
        std::cout << "CRITICAL ERROR" << std::endl;
        return;
    }

    for (unsigned int i = 0; i < weight.size() - 1; ++i)
    {
        sum += (weight[i] * prevLayerNeurons[i]->getOutput());
    }

    // const param
    sum += weight[weight.size() - 1];

    // Hyperbolic tangens
    output = tanh(BETA * sum);
}

void Neuron::calculateLastOutput(const std::vector<Neuron*> & prevLayerNeurons)
{
    output = 0;
    sum = 0;
    if ((weight.size() - 1) != prevLayerNeurons.size())
    {
        //CRITICAL ERROR
        std::cout << "CRITICAL ERROR" << std::endl;
        return;
    }
    for (unsigned int i = 0; i < weight.size() - 1; ++i)
    {
        sum += (weight[i] * prevLayerNeurons[i]->getOutput());
    }
    sum += weight[weight.size() - 1];
    output = sum;
}

void Neuron::calculateError(const std::vector<Neuron*> & nextNeuronLayer, const int& neuronNumber)
{
    error = 0;

    for (unsigned int i = 0; i < nextNeuronLayer.size(); ++i)
    {
        error += nextNeuronLayer[i]->getWeight(neuronNumber) * nextNeuronLayer[i]->getError();
    }
    error *= (BETA * (1 - (float)pow(tanh(BETA*sum), 2.0)));
}

void Neuron::correctWeight(const std::vector<Neuron*> & prevLayerNeurons)
{

    if ((weight.size() - 1) != prevLayerNeurons.size())
    {
        //CRITICAL ERROR
        std::cout << "CRITICAL ERROR" << std::endl;
        return;
    }
    float w;
    for (unsigned int i = 0; i < weight.size() - 1; ++i)
    {
        w = weight[i];
        weight[i] += ETA * error * prevLayerNeurons[i]->getOutput() + ALPHA*(weight[i] - prevWeight[i]);
        prevWeight[i] = w;
    }

    w = weight[weight.size() - 1];
    weight[weight.size() - 1] += ETA*error + ALPHA*(weight[weight.size() - 1] - prevWeight[weight.size() - 1]);
    prevWeight[weight.size() - 1] = w;
}

float Neuron::getWeight(const int & i)
{
    return weight[i];
}

float Neuron::getOutput()
{
    return output;
}

float Neuron::getError()
{
    return error;
}

void Neuron::setError(const float & result)
{
    error = (result - output);
}

void Neuron::setOutput(const float & out)
{
    output = out;
}

void Neuron::setParameters(const float alpha, const float beta, const float eta)
{
    ALPHA = alpha;
    BETA = beta;
    ETA = eta;
}