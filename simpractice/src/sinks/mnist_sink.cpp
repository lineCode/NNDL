#include <algorithm>
#include <cassert>
#include <fstream>
#include <ios>
#include <iostream>
#include <stdexcept>
#include <random>
#include <vector>

#include "signal.h"

#include "mnist_sink.h"



MnistSink::MnistSink()
{
    this->index = 0;
    this->load_labels();
    this->seed = 5;
    std::default_random_engine rnd(this->seed);
    shuffle(this->labels.begin(), this->labels.end(), rnd);
}

MnistSink::~MnistSink()
{
}

std::ostream& operator<<(std::ostream &outstream, const MnistSink &s)
{
    outstream << "Mnist Sink";
    return outstream;
}

std::vector<Signal> MnistSink::take(uint64_t time, std::vector<Signal> &outputs,
        const std::vector<Signal> &inputs)
{
    std::vector<Signal> one_hot_label = this->get_next_label();
    std::vector<Signal> diffs;

    assert((outputs.size() == NUM_NEURONS_OUTPUT));
    assert((one_hot_label.size() == NUM_NEURONS_OUTPUT));
    for (unsigned int i = 0; i < outputs.size(); i++)
    {
        Signal s = outputs.at(i);
        Signal label = one_hot_label.at(i);

        Signal dif = label - s;
        diffs.push_back(dif);
    }

    if (time % 10 == 0)
    {
        Signal err;
        for (unsigned int i = 0; i < diffs.size(); i++)
        {
            err += diffs.at(i) * diffs.at(i);
        }

        std::cout << "======ERROR for this iteration: " << err << "=========" << std::endl;
    }

    return one_hot_label;
}

std::vector<Signal> MnistSink::get_next_label()
{
    uint8_t label = this->labels.at(this->index);
    this->index++;

    //convert to one-hot
    std::vector<Signal> onehot;
    for (unsigned int i = 0; i < 10; i++)
    {
        if (label == i)
            onehot.push_back(Signal(1));
        else
            onehot.push_back(Signal(0));
    }

    return onehot;
}

void MnistSink::load_labels()
{
    std::string path("mnist/train-labels-idx1-ubyte");//This one is for training
    //std::string path("mnist/t10k-labels-idx1-ubyte");//This one is for testing


    auto reverse_int = [](int i)
    {
        unsigned char c1, c2, c3, c4;
        c1 = i & 0xFF;
        c2 = (i >> 8) & 0xFF;
        c3 = (i >> 16) & 0xFF;
        c4 = (i >>24) & 0xFF;
        return ((int)c1 << 24) + ((int)c2 << 16) + ((int)c3 << 8) + c4;
    };

    std::ifstream file(path, std::ios::binary);
    if (file.is_open())
    {
        int32_t magic_number = 0;
        int32_t num_labels = 0;

        //Read the first value into 'magic_number' to see if the file is valid
        file.read((char *)&magic_number, sizeof(magic_number));
        magic_number = reverse_int(magic_number);

        bool magic_valid = (magic_number == 2049);
        if (!magic_valid)
            throw std::runtime_error("Invalid MNIST label file.");

        file.read((char *)&num_labels, sizeof(num_labels));
        num_labels = reverse_int(num_labels);
        std::cout << "Number of labels: " << num_labels << std::endl;

        uint8_t label;
        for (int i = 0; i < num_labels; i++)
        {
            file.read((char *)&label, sizeof(label));
            this->labels.push_back(label);
        }
    }
    else
    {
        throw std::runtime_error("Cannot open file `" + path + "`");
    }
}










