/*
 * This is an example Sink for the MNIST dataset.
 */
#ifndef __MNIST_SINK_H__
#define __MNIST_SINK_H__

#include <vector>

#include "sink.h"

class MnistSink :
    public Sink
{
public:
    MnistSink();
    ~MnistSink();

    friend std::ostream& operator<<(std::ostream &outstream, const MnistSink &s);

    std::vector<Signal> take(uint64_t time, std::vector<Signal> &outputs,
            const std::vector<Signal> &inputs);

private:
    uint64_t index;
    std::vector<uint8_t> labels;
    unsigned int seed;

    uint8_t get_next_label();
    std::vector<Signal> convert_to_one_hot(uint8_t label);
    void load_labels();
    Signal soft_max(const std::vector<Signal> &vec) const;
};




#endif //header guard





