#include "common.h"


#ifndef M_PI
#define M_PI 3.14159265358979324
#endif


#pragma once

namespace Audio

{


struct Result
{
    float values[5];
};

struct ret
{
    ret(const std::vector<float>& values, int start, int end)
    {
        avg_sample = std::reduce(&values.at(start), &values.at(end)) / (end - start);
        auto max_it = std::max_element(&values.at(start), &values.at(end));
        position = static_cast<float>(std::distance(&values.at(start), max_it)) / (end - start);
        max_sample = *max_it;
    }

    float max_sample;
    float avg_sample;
    float position;
};



class FFTProcessor
{

public:

    FFTProcessor(tsqueue<sample_block>& stream);
    ~FFTProcessor();

    void Start();
    void Stop();

    tsqueue<sample_block>& GetProcessedSamples()
    { return m_processed; }

    void processBlock();

private:


    // stop condition
    std::atomic<bool> m_stop {false}; 
    
    // stream to process
    tsqueue<sample_block>& m_stream;

    // processed samples
    tsqueue<sample_block> m_processed;
    //tsqueue<Result> m_processed;

    // fft context
    kiss_fft_cfg m_cfg;
    kiss_fft_cpx m_cpxIN[BUFFER_SIZE];
    kiss_fft_cpx m_cpxOUT[BUFFER_SIZE];

    // delete
    float max_sample;
};


}