#include <recorder.h>

using namespace Audio;


AudioSettings::AudioSettings(const std::string& device, std::size_t buffer_size, unsigned int sample_rate, std::size_t channels) :
  device(device),
  buffer_size(buffer_size),
  sample_rate(sample_rate),
  channels(channels)
{}


Recorder::Recorder(const AudioConfiguration& config) :
    m_settings(AudioSettings( config.device, config.buffer_size, config.sample_rate, config.channels)),
    //m_recordingformat(SND_PCM_FORMAT_S32_LE)
    m_recordingformat(SND_PCM_FORMAT_S16_LE),
    m_state(State::notInitialized)
{}


Recorder::~Recorder()
{}

Recorder::State Recorder::GetState() const
{
    return m_state;
}

void Recorder::FreeResources()
{
    // free params
    snd_pcm_hw_params_free (m_hwparams);

    // free handle
    snd_pcm_close (m_handle);

    // update global config
    snd_config_update_free_global();

}

void Recorder::Start()
{
    m_state = State::Running;

    // keep running untill other thread tells to stop
    while (!m_stop.load(std::memory_order_relaxed))
    {
        // read data
        auto ret = snd_pcm_readi(m_handle, m_buffervec.data(), m_settings.buffer_size);

        // if ret not complete buffer stop
        if (ret != m_settings.buffer_size)
        {
            // update state
            m_state = State::notInitialized;
            std::cout << "Recording stoped, unvalid read size\n";
            break;
        }

        // store to thread-safe buffer
        sample_block data(std::begin(m_buffervec), std::end(m_buffervec));

        // thats obviously not working.. lets find max
        for (auto& d : data)
            d /= std::numeric_limits<s16>::max();
            
        m_tsbuffer.push_back(data);
    }

    FreeResources();

}

void Recorder::Stop()
{
    m_stop = true;
    // update state
    m_state = State::notInitialized;
}

bool Recorder::setup()
{
    // open audio device
    if ( snd_pcm_open (&m_handle, m_settings.device.c_str(), SND_PCM_STREAM_CAPTURE, 0) < 0)
    {
        std::cerr << "[ERROR]: Open audio device failed!\n";
        return false;
    }
        
    // allocate hw parameter structure
    if ( snd_pcm_hw_params_malloc (&m_hwparams) < 0)
    {
        std::cerr << "[ERROR]: hw param allocation failed!\n";
        return false;
    }
    // initialize hw param structure
    if ( snd_pcm_hw_params_any (m_handle, m_hwparams) < 0)
    {
        std::cerr << "[ERROR]: hw param init failed!\n";
        return false;
    }
    
    // set hw access
    if ( snd_pcm_hw_params_set_access (m_handle, m_hwparams, SND_PCM_ACCESS_RW_INTERLEAVED) < 0)
    {
        std::cerr << "[ERROR]: hw acces failed!\n";
        return false;
    }
    
    // set hw format
    if ( snd_pcm_hw_params_set_format (m_handle, m_hwparams, m_recordingformat) < 0)
    {
        std::cerr << "[ERROR]: invaild recording format provided!\n";
        return false;
    }

    // set sample rate
    if ( snd_pcm_hw_params_set_rate_near (m_handle, m_hwparams, &m_settings.sample_rate, 0) < 0)
    {
        std::cerr << "[ERROR]: invalid sample rate provided!\n";
        return false;
    }

    // set num channels
    if ( snd_pcm_hw_params_set_channels (m_handle, m_hwparams, m_settings.channels) < 0)
    {
        std::cerr << "[ERROR]: unsupported channel count provided!\n";
        return false;
    }

    // set hw params
    if ( snd_pcm_hw_params (m_handle, m_hwparams) < 0)
    {
        std::cerr << "[ERROR]: setting hw params failed!\n";
        return false;
    }
    
    // prepare handle
    if ( snd_pcm_prepare (m_handle) < 0)
    {
        std::cerr << "[ERROR]: handle preparation failed!\n";
        return false;
    }

    // prepare buffer
    m_buffervec.resize(m_settings.buffer_size);

    // set state
    m_state = State::Initialized;

    return true;
}

tsqueue<sample_block>& Recorder::getStream()
{
    return m_tsbuffer;
}



