#include "include/Audio.h"

int main()
{

    Audio::Streamer streamer("hw:1,0");
    auto& stream = streamer.GetFFTSamples();

    streamer.Start();
    std::this_thread::sleep_for(std::chrono::seconds(20));
    streamer.Stop();

    return 0;

}