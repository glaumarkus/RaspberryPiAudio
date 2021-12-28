#pragma once

#include "common.h"
#include "configreader.h"
#include "recorder.h"
#include "fft_processor.h"

namespace Audio {


class Streamer
{
public:
    Streamer(const AudioConfiguration& config);
    ~Streamer();

    void Start();
    void Stop();

    tsqueue<sample_block>& GetFFTSamples();

    Recorder::State GetState() const { return m_recorder.GetState(); }

    Recorder& GetRecorder() { return m_recorder; }
    FFTProcessor& GetProcessor() { return m_fft; }

private:

    // stop condition
    std::atomic<bool> m_stop {false}; 

    // Recording
    Recorder m_recorder;

    // Processor
    FFTProcessor m_fft;

    // threads
    std::thread m_recordingThread;
    std::thread m_processingThread;


};

}