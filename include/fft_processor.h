#include "common.h"
#include "configreader.h"

#ifndef M_PI
#define M_PI 3.14159265358979324
#endif


#pragma once

namespace Audio

{


class FFTProcessor
{

public:

    FFTProcessor(const AudioConfiguration& config, tsqueue<sample_block>& stream);
    ~FFTProcessor();

    void Start();
    void Stop();

    tsqueue<sample_block>& GetProcessedSamples()
    { return m_processed; }

    void processBlock();

    void UpdateDBScale(float dbscaler);
    void UpdateFFTScaler(float fftscaler);

    const int GetMaxHz () const { return (m_soi - 1) * m_config.sample_rate / m_config.buffer_size; }
    const float GetHzPerStep () const { return  static_cast<float>(m_config.sample_rate) / m_config.buffer_size; }

private:

    // stop condition
    std::atomic<bool> m_stop {false}; 
    
    // stream to process
    tsqueue<sample_block>& m_stream;

    // processed samples
    tsqueue<sample_block> m_processed;

    // fft context
    kiss_fft_cfg m_cfg;
    kiss_fft_cpx* m_cpxIN;
    kiss_fft_cpx* m_cpxOUT;

    // buffer sizes
    int m_bs;
    int m_soi;

    // config reference
    const AudioConfiguration& m_config;

    // mutable parameters
    float m_dbscaler;
    float m_fftscaler;

    // thread-safe blockers
    std::mutex muxDB;
    std::mutex muxFFT;
};


}