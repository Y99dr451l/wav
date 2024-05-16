#include <cstring>
#include "wav.h"

struct ChunkInfo {
    char chunkID[4];
    int32_t chunkSize;
};

struct RIFF {
    char chunkID[4];
    int32_t chunkSize;
    char format[4];
};

FMT::FMT(int32_t samplerate, int16_t channels) {
    audioFormat = 1;
    sampleRate = samplerate;
    numChannels = channels;
    bitsPerSample = sizeof(int16_t) * 8;
    blockAlign = numChannels * ((bitsPerSample + 7) / 8);
    byteRate = sampleRate * blockAlign;
    // extraParamSize = 0;
}

WAV::WAV(int32_t samplerate, int16_t channels, double length) {
    numSamples = length * samplerate;
    fmt = FMT(samplerate, channels);
    array = std::vector<std::vector<int16_t>>(channels, std::vector<int16_t>(numSamples));
}

WAV::WAV(std::vector<std::vector<int16_t>> array, int32_t samplerate) {
    numSamples = array[0].size();
    fmt = FMT(samplerate, array.size());
    this->array = array;
}

WAV::WAV(std::vector<int16_t> array, int32_t samplerate) {
    numSamples = array.size();
    fmt = FMT(samplerate, 1);
    this->array = std::vector<std::vector<int16_t>>(1, array);
}

WAV::WAV(const char* filename) {
    constexpr char riff_id[4] = {'R','I','F','F'};
    constexpr char format[4] = {'W','A','V','E'};
    constexpr char fmt_id[4] = {'f','m','t',' '};
    constexpr char data_id[4] = {'d','a','t','a'};

    FILE* file;
    file = fopen(filename, "rb");
    if (!file){
        std::cerr << "Cannot open file" << std::endl;
        return;
    }

    RIFF riff;
    fread((char*)(&riff), sizeof(riff), 1, file);
    if(!file || memcmp(riff.chunkID, riff_id, 4) || memcmp(riff.format, format, 4)) {
        std::cerr << "Bad formatting" << std::endl;
        return;
    }
    ChunkInfo ch;
    bool fmt_read = false, data_read = false;
    while(fread((char*)(&ch), sizeof(ch), 1, file)) {
        if(memcmp(ch.chunkID, fmt_id, 4) == 0) { // FMT
            fread((char*)(&fmt), ch.chunkSize, 1, file);
            fmt_read = true;
        } else if(memcmp(ch.chunkID, data_id, 4) == 0) { // DATA
            numSamples = ch.chunkSize / sizeof(int16_t) / fmt.numChannels;
            array = std::vector<std::vector<int16_t>>(fmt.numChannels, std::vector<int16_t>(numSamples));
            for (int i = 0; i < numSamples; i++)
                for (int j = 0; j < fmt.numChannels; j++)
                    fread((char*)&array[j][i], sizeof(int16_t), 1, file);
            data_read = true;
        } else fseek(file, ch.chunkSize, SEEK_CUR);
    }
    if (!data_read || !fmt_read) std::cout << "Problem when reading data" << std::endl;
    fclose(file);
}

std::ostream& operator<<(std::ostream& os, const FMT& fmt) {
    os << "audioFormat:    " << fmt.audioFormat << std::endl;
    os << "numChannels:    " << fmt.numChannels << std::endl;
    os << "sampleRate:     " << fmt.sampleRate << std::endl;
    os << "byteRate:       " << fmt.byteRate << std::endl;
    os << "blockAlign:     " << fmt.blockAlign << std::endl;
    os << "bitsPerSample:  " << fmt.bitsPerSample << std::endl;
    // os << "extraParamSize: " << fmt.extraParamSize << std::endl;
    return os;
}

std::ostream& operator<<(std::ostream& os, const WAV& wav) {
    os << wav.fmt;
    os << "numSamples:     " << wav.numSamples << std::endl;
    return os;
}

void WAV::write(const char* filename) {
    FILE* file = fopen(filename, "wb");
    int32_t fmtsize = sizeof(FMT);
    int32_t datasize = numSamples * fmt.numChannels * sizeof(int16_t);
    int32_t size = datasize + fmtsize + 36;
    RIFF riff = {{'R','I','F','F'}, size, {'W','A','V','E'}};
    fwrite((char*)&riff, sizeof(RIFF), 1, file);
    fwrite("fmt ", 4, 1, file);
    fwrite((char*)&fmtsize, sizeof(int32_t), 1, file);
    fwrite((char*)&fmt, sizeof(FMT), 1, file);
    fwrite("data", 4, 1, file);
    fwrite((char*)&datasize, sizeof(int32_t), 1, file);
    for (int i = 0; i < numSamples; i++)
        for (int j = 0; j < fmt.numChannels; j++)
            fwrite((char*)&array[j][i], sizeof(int16_t), 1, file);
    fclose(file);
}