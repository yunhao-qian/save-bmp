#define SAVE_BMP_IMPLEMENT
#include "save_bmp.h"

#include <vector>

int main() {
    const uint32_t width{1018}, height{822};
    std::vector<uint8_t> image(width * height * 3);
    auto channel{image.data()};
    for (uint32_t row{}; row < height; ++row)
        for (uint32_t column{}; column < width; ++column) {
            const auto r{static_cast<double>(row) / height};
            const auto g{static_cast<double>(column) / width};
            const auto b{0.5 * (r + g)};
            *channel++ = static_cast<uint8_t>(r * 255);
            *channel++ = static_cast<uint8_t>(g * 255);
            *channel++ = static_cast<uint8_t>(b * 255);
        }
    const auto result{save_bmp("example.bmp", width, height, image.data())};
    puts(save_bmp_str_result(result));
    return 0;
}
