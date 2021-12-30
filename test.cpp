#include "include/Audio.h"

int main()
{
    
    // load config file
    Audio::AudioConfiguration cc;
    if (!cc.LoadConfig("../config/audio.conf"))
    {
        std::cerr << "Error loading config file!";
        exit(1);
    }

    // init streamer
    Audio::Streamer streamer(cc);
    auto& stream = streamer.GetFFTSamples();

    // do some math beforehand
    auto& processor = streamer.GetProcessor();
    float hz_step = processor.GetHzPerStep();
    auto max_hz = processor.GetMaxHz();
    std::cout << "Max Frequency: " << max_hz << "\tResoltion: " << hz_step << std::endl;
    std::cout << "FPS: " << static_cast<float>(cc.sample_rate) / cc.buffer_size << "\n";
    bool cond = true;

    // start streamer with endpoint
    streamer.Start();
    auto run_until = std::chrono::system_clock::now() + std::chrono::seconds(2);

    while (std::chrono::system_clock::now() < run_until)
    {
        // wait for samples
        stream.wait();
        auto samples = stream.pop_front();

        // change inputs
        if (cond)
            processor.UpdateFFTScaler(9.0f);
    }

    streamer.Stop();

    std::cout << "Exiting...\n";


    return 0;

}