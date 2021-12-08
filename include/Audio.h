#pragma once

#include "common.h"
#include "recorder.h"
#include "fft_processor.h"

namespace Audio {


class Streamer
{
public:
    Streamer(const std::string& device);
    ~Streamer();

    void Start();
    void Stop();

    tsqueue<sample_block>& GetFFTSamples();

private:

    // stop condition
    std::atomic<bool> m_stop {false}; 

    // Recording
    A_Recorder m_recorder;

    // Processor
    FFTProcessor m_fft;

    // threads
    std::thread m_recordingThread;
    std::thread m_processingThread;


};

}