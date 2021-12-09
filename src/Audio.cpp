#include <Audio.h>

using namespace Audio;

Streamer::Streamer(const std::string& device) :
    m_recorder(
        AudioSettings(
            device,
            Audio::BUFFER_SIZE,
            Audio::SAMPLE_RATE,
            1
        )
    ),
    m_fft(m_recorder.getStream())
{
    if (!m_recorder.setup())
        throw("[ERROR]: Setting up audio device failed, program exits!\n");
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
