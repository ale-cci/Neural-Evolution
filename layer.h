#pragma once
#include <iostream>
#include "neuron.h"

uint16_t  nneurons(const uint16_t layer_id);
uint16_t anneurons(const uint16_t layer_id);
uint16_t add_neuron(const uint16_t layer_id);

struct LAYER {
    uint16_t layer_id;
    struct NEURON* neuron;

    void init(const uint16_t layer_id);
    void execute(NEURON* nextlayer);
    void duplicate(LAYER* father);
    void destroy();
    void refresh();
    void checknan();

    void mutate(NEURON* nextlayer);
    void print(std::ofstream& out);
    void read(std::ifstream& in);
    void write(std::ofstream& out);
    LAYER& operator=(const LAYER& ) noexcept;
};
