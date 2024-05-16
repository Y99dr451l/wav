#ifndef WAV_H
#define WAV_H

#include <cstdint>
#include <iostream>
#include <vector>

struct FMT {
    int16_t audioFormat;
    int16_t numChannels;
    int32_t sampleRate;
    int32_t byteRate;
    int16_t blockAlign;
    int16_t bitsPerSample;
    // int16_t extraParamSize;

    FMT() {}
    FMT(int32_t samplerate, int16_t channels);
};

struct WAV {
    FMT fmt;
    int32_t numSamples;
    std::vector<std::vector<int16_t>> array;

    WAV() : fmt() {}
    WAV(int32_t samplerate, int16_t channels, double length);
    WAV(std::vector<std::vector<int16_t>> array, int32_t samplerate);
    WAV(std::vector<int16_t> array, int32_t samplerate);
    WAV(const char* filename);
    ~WAV() {}

    friend std::ostream& operator<<(std::ostream& os, const WAV& wav);
    friend FILE* operator<<(FILE* file, const WAV& wav);
    inline std::vector<int16_t>& operator[](int32_t index) { return array[index]; }
    inline const std::vector<int16_t> operator[](int32_t index) const { return array[index]; }

    void write(const char* filename);
};

#endif