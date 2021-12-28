#include <Audio.h>

using namespace Audio;

Streamer::Streamer(const AudioConfiguration& config) :
    m_recorder(config),
    m_fft(config, m_recorder.getStream())
{
    if (!m_recorder.setup())
        throw std::runtime_error("[ERROR]: Audio device does not exist or wrong parameters, program exits!\n");

}

Streamer::~Streamer()
{
    m_recorder.Stop();
    m_fft.Stop();

    if (m_recordingThread.joinable())
        m_recordingThread.join();
    if (m_processingThread.joinable())
        m_processingThread.join();
}

void Streamer::Start()
{
    m_recordingThread = std::thread(
        [this]()
        {
            m_recorder.Start();
        }
    );

    m_processingThread = std::thread(
        [this]()
        {
            m_fft.Start();
        }
    );

}

void Streamer::Stop()
{
    m_recorder.Stop();
    m_fft.Stop();

}

tsqueue<sample_block>& Streamer::GetFFTSamples()
{
    return m_fft.GetProcessedSamples();
}
