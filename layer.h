#pragma once
#include "neuron.h"

struct LAYER {
    int8_t neuron_number;
    struct NEURON* neuron;

    void init(LAYER * nextlayer, int8_t number_of_neurons);
    void execute(LAYER* nextlayer);
    void refresh();
    void inherit_from(LAYER* father, LAYER* nextlayer);

    void mutate(LAYER* nextlayer);
};
