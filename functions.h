#include <vector>

inline std::vector<int16_t> rect(int32_t length) {
    std::vector<int16_t> result(length);
    for (int32_t i = 0; i < length; i++)
        result[i] = i < length / 2 ? 1 : -1;
    return result;
}

inline std::vector<int16_t> triangle(int32_t length) {
    std::vector<int16_t> result(length);
    for (int32_t i = 0; i < length; i++)
        result[i] = i < length / 2 ? i : length - i;
    return result;
}