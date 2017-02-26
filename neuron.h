#pragma once
#include "generic_functions.h"
#include "neural_network_constant.h"
#include "layer.h"

struct NEURON {
    int input_nums;
    _PRECISION input_sum;
    _PRECISION prop_value; // precision value of the neuron
    _PRECISION *synapsy;
    void input(_PRECISION in);
    void init(struct LAYER* nextlayer);
    void execute(struct LAYER* nextlayer);
    void refresh();
    void inherit_from(NEURON* father, LAYER* nextlayer);
    void mutate(struct LAYER* nextlayer);
    double spread_value();
};
