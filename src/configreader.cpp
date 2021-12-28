#include <configreader.h>


using namespace Audio;

AudioConfiguration::AudioConfiguration()
{}

AudioConfiguration::~AudioConfiguration()
{}

bool AudioConfiguration::LoadConfig(const std::string& path)
{

    std::ifstream file_(path);

    if (!file_.is_open()) {
      std::cerr << "Config file not found!" << std::endl;
      return false;
    }

    std::string line_;
    int i = 0;

    while (getline(file_, line_)) 
    {
        if (line_[0] == '#') continue;
        if (line_.empty()) continue;

        std::stringstream param(line_);
        std::string paramName;

        param >> paramName;
        if (paramName == "device")
            param >> device;
        else if (paramName == "channels") 
            param >> channels;
        else if (paramName == "sample_rate") 
            param >> sample_rate;
        else if (paramName == "buffer_size") 
            param >> buffer_size;
        else if (paramName == "magnitude_divisor") 
            param >> magnitude_divisor;
        else if (paramName == "db_scaler") 
            param >> db_scaler;    
        else 
            std::cerr << "Unrecognized pipeline parameter: " << paramName << std::endl;
    }
    file_.close();
    return true;
}
