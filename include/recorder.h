#pragma once

#include "common.h"


namespace Audio
{



struct AudioSettings
{
  AudioSettings(const std::string& device, std::size_t buffer_size, unsigned int framerate, std::size_t channels);

  std::string device;
  std::size_t channels;
  std::size_t buffer_size;
  unsigned int framerate;
};



class A_Recorder : std::enable_shared_from_this<A_Recorder>
{
public:

  enum class State : uint8_t
  {
    notInitialized,
    Initialized,
    Running
  };
  
  A_Recorder(const AudioSettings& settings);

  ~A_Recorder();
  
  bool setup();
  void Start();
  void Stop();

  State GetState() const;

  tsqueue<sample_block>& getStream();

  const AudioSettings& GetSettings() const { return m_settings;}

private:
  
  void FreeResources();

  State m_state;
  
  std::atomic<bool> m_stop {false}; 
  std::vector<s16> m_buffervec;
  s16* m_buffer;
  std::size_t m_buffersize;

  tsqueue<sample_block> m_tsbuffer;

  snd_pcm_t* m_handle;
  snd_pcm_hw_params_t* m_hwparams;
  snd_pcm_format_t m_recordingformat;

  AudioSettings m_settings;

};








}


