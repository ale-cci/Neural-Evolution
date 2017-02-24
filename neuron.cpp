#include <algorithm>
#include <fstream>
#include <iostream>
#include "neuron.h"
#include "generic_functions.h"

void NEURON::execute(LAYER* next_layer) {
    for (int i=0; i < next_layer->neuron_number; ++i) {
        if (synapsy[i] <= 0) // not connected
            continue;
        // broadcast his value
        next_layer->neuron[i].input( spread_value() * synapsy[i]);
    }
    return ;
}
double NEURON::spread_value() {
    return special_one(prop_value, input_sum /  std::max(1, input_nums));
}
void NEURON::input(_PRECISION in) {
    input_sum += in;
    return ;
}

void NEURON::refresh() {
    input_sum = 0;
    return ;
}
void NEURON::init(LAYER* nextlayer) {
    input_nums = 0;
    prop_value = rand(0.0, 1.0);
    if (nextlayer == nullptr)
        return ;
    synapsy = new _PRECISION[nextlayer->neuron_number];
    for (int i = 0; i < nextlayer->neuron_number; ++i) {
        synapsy[i] = std::max(0.0, rand(-0.5, 1.5)); // 1/3 no synapsy
        //std::cout << synapsy[i] << std::endl;
        if (synapsy[i])
            nextlayer->neuron[i].input_nums++;
    }
}
void NEURON::inherit_from(NEURON* father, LAYER* nextlayer) {

    input_nums = father->input_nums;
    prop_value = father->prop_value;
    if (nextlayer == nullptr)
        return ;
    for (int i=0; i < nextlayer->neuron_number; ++i)
        synapsy[i] = father->synapsy[i];
}

void NEURON::save(std::ofstream& out, LAYER* nextlayer) {
    out << prop_value << std::endl;
    for (int i = 0; i < nextlayer->neuron_number; ++i)
        out << synapsy[i] << " ";
    out << std::endl;
}

void NEURON::load(std::ifstream& in, LAYER* nextlayer) {
    in >> prop_value;
    for (int i = 0; i < nextlayer->neuron_number; ++i)
        in >> synapsy[i];
}

void NEURON::mutate(LAYER* nextlayer) {
    if (rand(0, 100) < MUTATION_PERCENTAGE)
        prop_value += rand(-MUTATION_CHANGE,+MUTATION_CHANGE);
    if (nextlayer == nullptr)
        return ;
    for (int i = 0; i < nextlayer->neuron_number; ++i)
        if (rand(0, 100) < MUTATION_PERCENTAGE) {
            double mutation = rand(-MUTATION_CHANGE,+MUTATION_CHANGE);
            if ((synapsy[i] > 0) && (synapsy[i] + mutation <= 0)) {
                nextlayer->neuron[i].input_nums--;
                synapsy[i] = 0;
            }
            else
            if ((synapsy[i] == 0) && (synapsy[i] + mutation > 0)) { // TODO check better this condition
                nextlayer->neuron[i].input_nums++;
                synapsy[i] += mutation;
            }
            else {
                synapsy[i] += mutation;
                if (synapsy[i] > 1)
                    synapsy[i] = 1;
            }
        }

}
