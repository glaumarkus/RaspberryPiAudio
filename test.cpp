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
    bool cond = true;

    // start streamer with endpoint
    streamer.Start();
    auto run_until = std::chrono::system_clock::now() + std::chrono::seconds(10);

    while (std::chrono::system_clock::now() != run_until)
    {
        // wait for samples
        stream.wait();
        auto samples = stream.pop_front();

        // change inputs
        if (cond)
            processor.UpdateFFTScaler(9.0f);
    }

    streamer.Stop();


    return 0;

}