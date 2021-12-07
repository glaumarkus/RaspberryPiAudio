#include <fft_processor.h>

using namespace Audio;

FFTProcessor::FFTProcessor(tsqueue<sample_block>& stream) :
    m_stream(stream)
{
    m_cfg = kiss_fft_alloc( BUFFER_SIZE, 0, 0, 0);
    if (m_cfg == NULL)
        throw("FFT config setup failed!");
}

FFTProcessor::~FFTProcessor()
{
    //kiss_fft_free(m_cfg);
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

void print_state(const float& state)
{
    int sig_str = state * 10;
    if (sig_str > 10)
        sig_str = 10;
    std::cout << "[";
    for (int i = 0; i < 10; i++)
    {
        if (i < sig_str)
            std::cout << "x";
        else
            std::cout << " ";
    }
    std::cout << "]\t";
}



// ToDo: add info on sample rate per second to convert frequencies
void FFTProcessor::processBlock()
{

    // wait for new sample to process
    m_stream.wait();

    // pop sample from queue
    auto nextBlock = m_stream.pop_front();

    // copy samples to complex array
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        m_cpxIN[i].r = nextBlock[i];
        m_cpxIN[i].i = 0;
    }

    // fft of samples
    kiss_fft(m_cfg, m_cpxIN, m_cpxOUT);

    // calculate magnitudes
    std::vector<float> magnitudes;
    magnitudes.reserve(BUFFER_SIZE_HALF);

    // calculate phases
    std::vector<float> phases;
    phases.reserve(BUFFER_SIZE_HALF);

    // calculations 
    for (int i = 0; i < BUFFER_SIZE_HALF; i++)
    {
        magnitudes.push_back(
            sqrt(m_cpxOUT[i].r * m_cpxOUT[i].r + m_cpxOUT[i].i * m_cpxOUT[i].i)
        );
        phases.push_back(
            atan2f32(m_cpxOUT[i].i, m_cpxOUT[i].r)
        );
    }

#ifdef CHECK_LEVEL

    // find max magnitude, replace with set value
    float max_m = *std::max_element(magnitudes.begin(), magnitudes.end());

    if (max_m > max_sample)
        max_sample = max_m;

    std::cout << "Sample: " << max_sample << "\n";

#endif 

    // normalize magnitudes
    for (auto& m : magnitudes)
        m /= FFT_DIVISOR;

        
    //  convert to log scale
    for (int i = 0; i < BUFFER_SIZE_HALF; i++)
    {
        auto tmp = FFT_DB_SCALER * log10f32(magnitudes[i]);
        if (tmp < -30.0f)
            tmp = -30.0f;
        else if (tmp > 0.0f)
            tmp = 0.0f;
        magnitudes[i] = (tmp + 30) / 30.0f;
    }
    
#ifdef CHECK_FFT_OUTPUT
    std::vector<float>::iterator max_mm = std::max_element(magnitudes.begin(), magnitudes.end());
    std::cout << "Max: " << (*max_mm) << "\n";
#endif

    m_processed.push_back(magnitudes);

    /*
    // group in containers
    const int BASS = 12;
    const int LOW = 24;
    const int MID = 93;
    const int HIGH = 186;
    const int PRESENCE = 279;

    ret bass        (magnitudes, 0, BASS);
    ret low         (magnitudes, BASS, LOW);
    ret mid         (magnitudes, LOW, MID);
    ret high        (magnitudes, MID, HIGH);
    ret presence    (magnitudes, HIGH, PRESENCE);

    constexpr int DIVISOR = 10;
    
    // print averages
    print_state(bass.max_sample / DIVISOR);
    print_state(low.max_sample / DIVISOR);
    print_state(mid.max_sample / DIVISOR);
    print_state(high.max_sample / DIVISOR);
    print_state(presence.max_sample / DIVISOR);


    std::cout << "\r";
    std::cout.flush();
    */

}