// #include <cstdint>
#include <iostream>
// #include <cmath>

#include "wav.h"
#include "functions.h"

void convolve(std::vector<int16_t>& data, std::vector<int16_t>& kernel, std::vector<int16_t>& result) {
    for (uint32_t i = 0; i < result.size(); i++) {
        if (i % 500 == 0) std::cout << i << std::endl;
        result[i] = 0;
        for (uint32_t j = 0; j < kernel.size(); j++)
            if (i >= j && i < data.size() + j)
                result[i] += kernel[j] * data[i - j];
    }
}

std::vector<int16_t> convolve(std::vector<int16_t>& data, std::vector<int16_t>& kernel) {
    std::vector<int16_t> result(data.size());
    convolve(data, kernel, result);
    return result;
}

int main2() {
    WAV wav = WAV("water-splash-1.wav");
    std::cout << wav << std::endl;
    WAV wav2 = WAV("EchoThiefImpulseResponseLibrary/Underpasses/HopkinsDriveUnderpass.wav");
    std::cout << wav2 << std::endl;

    std::vector<int16_t> kernel = wav2[0];
    for (uint32_t j = 0; j < kernel.size(); j++) kernel[j] = kernel[j] / 100;
    std::vector<int16_t> data = wav[0];
    for (uint32_t j = 0; j < data.size(); j++) data[j] = data[j] / 100;
    std::vector<int16_t> result(wav.numSamples);
    convolve(data, kernel, result);

    WAV wav3 = WAV(wav.fmt.sampleRate, 1, wav.numSamples / (double)wav.fmt.sampleRate);
    for (int32_t j = 0; j < wav3.numSamples; j++) wav3[0][j] = result[j];
    std::cout << wav3;
    wav3.write("test3.wav");
    return 0;
}

int main() {
    std::vector<int16_t> vtri = triangle(100);
    int32_t len = vtri.size(), reps = 101;
    std::vector<int16_t> result(len*reps);
    for (int32_t i = 0; i < reps; i++) {
        std::cout << i << std::endl;
        for (int32_t j = 0; j < len; j++) {
            result[i*reps + j] = 1000 * vtri[j];
        }
    }
    std::cout << "convolved" << std::endl;
    WAV wav = WAV(result, 44100);
    wav.write("conv.wav");
    std::cout << wav;
    return 0;
}