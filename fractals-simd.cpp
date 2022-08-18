#include "fractals-simd.h"

namespace fractals::utils::colormap {

int32_t gray(unsigned char value)
{
    int32_t v = value;
    return 0xff000000 | v << 16 | v << 8 | v;
}

int32_t red(unsigned char value)
{
    return int32_t(value) << 16;
}

int32_t green(unsigned char value)
{
    return int32_t(value) << 8;
}

int32_t blue(unsigned char value)
{
    return int32_t(value);
}

int32_t hot(unsigned char value)
{
    uint32_t red = 255 * std::min(1.f, value / 95.f);
    uint32_t green = value > 96 ? 255 * std::min(1.f, (value - 96) / 95.f) : 0;
    uint32_t blue = value > 192 ? 255 * (value - 192) / 63 : 0;
    return red << 16 | green << 8 | blue;
}

}
