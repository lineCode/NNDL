/*
 * This is the network ADT - it is class that holds information and functions
 * relating to the Neural Network created by the NNDL file.
 */
#ifndef __NETWORK_H__
#define __NETWORK_H__

#include <iostream>
#include <set>

#include "connection_map.h"
#include "layer.h"
#include "neuron.h"
#include "synapse.h"
#include "unit_tests.h"

class Network
{
public:
    Network();
    Network(const Network &to_copy);
    Network(std::vector<Layer *> *layers, std::vector<Synapse *> *connections);
    ~Network();

    friend std::ostream& operator<<(std::ostream &outstream, const Network &nw);

    Network& operator=(const Network &rhs);

    /*
     * Loads the given vector of Signals (which must of size equal to
     * to the output layer of the network) into the output layer
     * and triggers each Synapse backwards.
     *
     * The result is a vector of Signals coming from the input layer.
     */
    std::vector<Signal> fire_backward(uint64_t time, std::vector<Signal> input);

    /*
     * This is the standard mechanism for computing a result from the network.
     * The input must be of size equal to the number of input neurons.
     */
    std::vector<Signal> fire_forward(uint64_t time, std::vector<Signal> input);

    /*
     * Gets the weights in the network. Suitable for printing them as
     * one long list.
     */
    void get_weights(std::vector<Signal> &weights) const;

    static UnitTestResult run_tests();


private:
    std::vector<Layer *> *layers;
    ConnectionMap connection_map;
    std::vector<Neuron *> top_sorted_network;

    Layer* get_input_layer() const;
    Layer* get_output_layer() const;

    std::vector<std::tuple<Neuron *, Signal>>& filter_for_input_neurons(
            std::vector<std::tuple<Neuron *, Signal>> &outputs,
            std::vector<std::tuple<Neuron *, Signal>> &input_neurons) const;

    std::vector<std::tuple<Neuron *, Signal>>& filter_for_output_neurons(
            std::vector<std::tuple<Neuron *, Signal>> &outputs,
            std::vector<std::tuple<Neuron *, Signal>> &output_neurons) const;

    std::set<Neuron *> get_all_nodes_no_incoming();

    std::vector<Signal>& get_node_inputs(uint64_t time, const Neuron *n,
            std::vector<std::tuple<Neuron *, Signal>> &outputs,
            std::vector<Signal> &inputs,
            const std::vector<Signal> &network_input);

    std::vector<Signal>& get_node_inputs_backward(uint64_t time, const Neuron *n,
            std::vector<std::tuple<Neuron *, Signal>> &outputs,
            std::vector<Signal> &inputs,
            const std::vector<Signal> &network_input);

    void initialize_connection_map(std::vector<Synapse *> *connections);

    bool is_all_synapses(const std::set<Synapse *> &graph);

    std::vector<Signal>& map_to_input(std::vector<std::tuple<
            Neuron *, Signal>> &input_neurons, std::vector<Signal> &output);

    std::vector<Signal>& map_to_output(std::vector<std::tuple<
            Neuron *, Signal>> &output_neurons, std::vector<Signal> &output);

    bool neuron_is_input_neuron(const Neuron *n) const;
    bool neuron_is_output_neuron(const Neuron *n) const;

    std::vector<Neuron *> topological_sort();


    static void test_neuron_is_output_neuron(UnitTestResult &result);
    static void test_topological_sort(UnitTestResult &result);
    static void create_test_network(Network &test_network);
};



#endif //header guard



