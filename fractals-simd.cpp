#include "fractals-simd.h"

namespace fractals::utils {

namespace colormap {

uint32_t gray(unsigned char value)
{
    uint32_t v = value;
    return v << 16 | v << 8 | v;
}

uint32_t red(unsigned char value)
{
    return uint32_t(value) << 16;
}

uint32_t green(unsigned char value)
{
    return uint32_t(value) << 8;
}

uint32_t blue(unsigned char value)
{
    return uint32_t(value);
}

uint32_t hot(unsigned char value)
{
    uint32_t red = 255 * std::min(1.f, value / 95.f);
    uint32_t green = value > 96 ? 255 * std::min(1.f, (value - 96) / 95.f) : 0;
    uint32_t blue = value > 192 ? 255 * (value - 192) / 63 : 0;
    return red << 16 | green << 8 | blue;
}

uint32_t jet(unsigned char value)
{
    float red, green, blue;

    // red
    if (value < 96)
        red = 0;
    else if (value < 160)
        red = (value - 96) / 64.f;
    else if (value < 224)
        red = 1;
    else
        red = 1 - (value - 224) / 64;

    // green
    if (value < 32)
        green = 0;
    else if (value < 96)
        green = (value - 32) / 64.f;
    else if (value < 160)
        green = 1;
    else if (value < 224)
        green = 1 - (value - 160) / 64.f;
    else
        green = 0;

    // blue
    if (value < 32)
        blue = 0.5f + value / 64.f;
    else if (value < 96)
        blue = 1;
    else if (value < 160)
        blue = 1 - (value - 96) / 64.f;
    else
        blue = 0;

    return uint32_t(std::round(red * 255)) << 16
         | uint32_t(std::round(green * 255)) << 8
         | uint32_t(std::round(blue * 255));
}

}

namespace transforms {

Point<uint32_t> identity(const Buffer& buffer, const Point<uint32_t>& xy)
{
    return xy;
}

Point<uint32_t> horizontal_flip(const Buffer& buffer, const Point<uint32_t>& xy)
{
    return {buffer.width() - xy.x() - 1, xy.y()};
}

Point<uint32_t> vertical_flip(const Buffer& buffer, const Point<uint32_t>& xy)
{
    return {xy.x(), buffer.height() - xy.y() - 1};
}

}

void save_buffer_as_bmp(const Buffer& buffer, const std::string& filename, const Colormap& colormap, const Transform& transform)
{
    std::ofstream file(filename, std::ios::binary);

    uint32_t filesize = (14 + 40) + 4 * buffer.width() * buffer.height();

    uint8_t header[14] = {0};
    header[0] = 'B';
    header[1] = 'M';
    *reinterpret_cast<uint32_t*>(&header[2]) = filesize;
    *reinterpret_cast<uint32_t*>(&header[6]) = 0;
    *reinterpret_cast<uint32_t*>(&header[10]) = 54;

    uint8_t info_header[40] = {0};
    *reinterpret_cast<uint32_t*>(&info_header[0]) = sizeof(info_header);
    *reinterpret_cast<uint32_t*>(&info_header[4]) = buffer.width();
    *reinterpret_cast<uint32_t*>(&info_header[8]) = buffer.height();
    *reinterpret_cast<uint16_t*>(&info_header[12]) = 1;
    *reinterpret_cast<uint16_t*>(&info_header[14]) = 32;

    file.write(reinterpret_cast<char*>(header), sizeof(header));
    file.write(reinterpret_cast<char*>(info_header), sizeof(info_header));

    // Save data
    uint32_t max_value = 0;
    for (uint32_t y = 0; y < buffer.height(); ++y)
    {
        for (uint32_t x = 0; x < buffer.width(); ++x)
        {
            max_value = std::max(max_value, *buffer.ptr(x, y));
        }
    }

    auto zero = char(0);
    for (uint32_t y = 0; y < buffer.height(); ++y)
    {
        for (uint32_t x = 0; x < buffer.width(); ++x)
        {
            auto transformed_xy = transform(buffer, {x, buffer.height() - 1 - y});
            uint32_t value = (max_value > 0 ? colormap(std::round(255.0f * *buffer.ptr(transformed_xy.x(), transformed_xy.y()) / max_value)) : 0);
            file.write(reinterpret_cast<char*>(&value), 4);
        }
    }

    file.close();
}

}
