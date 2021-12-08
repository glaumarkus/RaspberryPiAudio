#include "include/Audio.h"

int main()
{

    Audio::Streamer streamer("hw:1,0");
    auto& stream = streamer.GetFFTSamples();

    streamer.Start();
    std::this_thread::sleep_for(std::chrono::seconds(60));
    streamer.Stop();

    /*
    // Audio Capture Settings
    Audio::AudioSettings settings
    {
        "hw:1,0",
        Audio::BUFFER_SIZE,
        Audio::SAMPLE_RATE,
        2
    };

    // Audio Capture 
    Audio::A_Recorder recorder(settings);
    if (!recorder.setup())
        std::cerr << "[ERROR] Setting up audio device failed!\n";
    
    // FFT processor
    Audio::FFTProcessor processor(recorder.getStream());

    // Recording thread
    std::thread reader(
        [&recorder]()
        {
            recorder.Start();
        }
    );

    // Processing thread
    std::thread fft_thread(
        [&processor]()
        {
            processor.Start();
        }
    );
    
    // wait
    std::this_thread::sleep_for(std::chrono::seconds(60));

    // close
    recorder.Stop();
    processor.Stop();

    // join
    if (reader.joinable())
        reader.join();
    if (fft_thread.joinable())
        fft_thread.join();
    */

    return 0;

}