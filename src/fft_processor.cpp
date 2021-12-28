#include <fft_processor.h>

using namespace Audio;

FFTProcessor::FFTProcessor(const AudioConfiguration& config, tsqueue<sample_block>& stream) :
    m_stream(stream),
    m_config(config)
{
    // save size
    m_bs = m_config.buffer_size;
    m_soi = m_bs / 2;
    
    m_cfg = kiss_fft_alloc( m_bs, 0, 0, 0);
    if (m_cfg == NULL)
        throw("FFT config setup failed!");
    
    // allocate arrays 
    m_cpxIN = new kiss_fft_cpx[m_bs];
    m_cpxOUT = new kiss_fft_cpx[m_bs];

    // mutable starting params
    m_fftscaler = 8.0f;
    m_dbscaler = 20.0f;

}

FFTProcessor::~FFTProcessor()
{
    delete[] m_cpxIN;
    delete[] m_cpxOUT;
}

void FFTProcessor::Start()
{
    while (!m_stop)
        processBlock();
}

void FFTProcessor::Stop()
{
    m_stop = true;
}

void FFTProcessor::UpdateDBScale(float dbscaler)
{
    std::scoped_lock lock(muxDB);
    m_dbscaler = dbscaler;
}

void FFTProcessor::UpdateFFTScaler(float fftscaler)
{
    std::scoped_lock lock(muxFFT);
    m_fftscaler = fftscaler;
}

// ToDo: add info on sample rate per second to convert frequencies
void FFTProcessor::processBlock()
{
    // wait for new sample to process
    m_stream.wait();

    // pop sample from queue
    auto nextBlock = m_stream.pop_front();

    // copy samples to complex array
    for (int i = 0; i < m_bs; i++)
    {
        m_cpxIN[i].r = nextBlock[i];
        m_cpxIN[i].i = 0;
    }

    // fft of samples
    kiss_fft(m_cfg, m_cpxIN, m_cpxOUT);

    // calculate magnitudes
    std::vector<float> magnitudes;
    magnitudes.reserve(m_soi);

    // calculations 
    for (int i = 0; i < m_soi; i++)
        magnitudes.push_back(
            sqrt(m_cpxOUT[i].r * m_cpxOUT[i].r + m_cpxOUT[i].i * m_cpxOUT[i].i)
        );

    // normalize magnitudes
    for (auto& m : magnitudes)
        m /= m_fftscaler;

    //  convert to log scale
    for (int i = 0; i < m_soi; i++)
    {
        auto tmp = m_dbscaler * log10f32(magnitudes[i]);
        if (tmp < -30.0f)
            tmp = -30.0f;
        else if (tmp > 0.0f)
            tmp = 0.0f;
        magnitudes[i] = (tmp + 30) / 30.0f;
    }

    m_processed.push_back(magnitudes);
}