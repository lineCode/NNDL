/*
 * Header for a Layer ADT.
 * A Layer is essentially a vector of Neurons.
 */

#ifndef __LAYER_H__
#define __LAYER_H__

#include "neuron.h"

class Layer
{
public:
    Layer();
    ~Layer();

    Neuron& operator[] (const int index);

    void add_neuron(const Neuron &n);
    Neuron& at(int index);

private:
    std::vector<Neuron> neurons;
};



#endif //header guard








