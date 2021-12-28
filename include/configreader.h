#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <memory>

namespace Audio {
    

struct AudioConfiguration
{
    AudioConfiguration();
    ~AudioConfiguration();

    bool LoadConfig(const std::string& path);

    std::string device = "";
    uint8_t channels = 0;
    uint32_t sample_rate = 0;
    uint32_t buffer_size = 0;
    double magnitude_divisor = 0;
    double db_scaler = 0;
};

}
