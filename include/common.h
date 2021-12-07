#pragma once

#include <iostream>
#include <vector>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <string>
#include <thread>
#include <atomic>
#include <algorithm>
#include <memory>
#include <math.h>
#include <numeric>
#include <stdio.h>
#include <cstdint>
#include <chrono>
#include <map>
#include <iterator>

// extern
#include <alsa/asoundlib.h>
#include "../dependencies/kissfft/kiss_fft.h"

// intern
#include "tsqueue.h"

// #define CHECK_LEVEL
#define CHECK_FFT_OUTPUT

namespace Audio {

    
typedef int16_t s16;
typedef float sample;
typedef std::vector<sample> sample_block;

constexpr int   SAMPLE_RATE = 44100;
constexpr int   BUFFER_SIZE = 1024;
constexpr int   BUFFER_SIZE_HALF = BUFFER_SIZE / 2;
constexpr float FFT_DIVISOR = 5;
constexpr float FFT_DB_SCALER = 15.0f;

}

